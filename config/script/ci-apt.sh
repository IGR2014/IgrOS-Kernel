#!/usr/bin/env bash
#
#	Install CI packages with apt-get: every phase has its own time limit and
#	is retried, so a stalled mirror connection gets a fresh one instead of
#	hanging the job (usage: ci-apt.sh package...)
#
#	File:	ci-apt.sh
#	Date:	22 Oct 2026
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


set -u

ATTEMPTS=3
UPDATE_TIMEOUT=90
INSTALL_TIMEOUT=180

export DEBIAN_FRONTEND=noninteractive

# Phase start with timestamp (shows in job log which phase was slow)
phase() {
	echo "[ci-apt $(date -u +%H:%M:%S)] $*"
}

# apt options: retries, connection timeouts, IPv4 only, wait for dpkg lock (unattended upgrades)
APT=(
	sudo -E apt-get
	-o Acquire::Retries=3
	-o Acquire::http::Timeout=30
	-o Acquire::https::Timeout=30
	-o Acquire::ForceIPv4=true
	-o DPkg::Lock::Timeout=120
	-o Dpkg::Use-Pty=0
)

LOG="$(mktemp)"

# Run apt phase with time limit, output to job log and LOG
# (on failure its tail goes to a warning annotation: readable in run summary and API)
run() {
	local limit="$1" name="$2"
	shift 2
	timeout --kill-after=10 "${limit}" "$@" 2>&1 | tee "${LOG}"
	local status="${PIPESTATUS[0]}"
	if [[ "${status}" -ne 0 ]]; then
		local reason="exit ${status}"
		[[ "${status}" -eq 124 ]] && reason="timed out after ${limit} s"
		echo "::warning title=ci-apt attempt ${attempt} ${name} ${reason}::$(tail -n 15 "${LOG}" | sed -e 's/%/%25/g' | sed -e ':a;N;$!ba;s/\n/%0A/g')"
	fi
	return "${status}"
}

# Use main Ubuntu archive instead of runner's (Azure) mirror: runner images
# list it in mirror+file:/etc/apt/apt-mirrors.txt, older ones in sources
switchMirror() {
	phase "switching to archive.ubuntu.com"
	sudo sed -i -e 's#https\?://azure\.archive\.ubuntu\.com#https://archive.ubuntu.com#g' \
		/etc/apt/apt-mirrors.txt /etc/apt/sources.list /etc/apt/sources.list.d/*.list /etc/apt/sources.list.d/*.sources 2>/dev/null || true
}

# Don't rebuild man page index after install (slow on runner images)
sudo rm -f /var/lib/man-db/auto-update

for ((attempt = 1; attempt <= ATTEMPTS; attempt++)); do
	# Runner mirror failed once: try main archive
	[[ "${attempt}" -eq 2 ]] && switchMirror
	phase "attempt ${attempt}/${ATTEMPTS}: update"
	if run "${UPDATE_TIMEOUT}" update "${APT[@]}" update; then
		phase "attempt ${attempt}/${ATTEMPTS}: install $*"
		if run "${INSTALL_TIMEOUT}" install "${APT[@]}" install -y --no-install-recommends "$@"; then
			phase "done"
			exit 0
		fi
	fi
	phase "attempt ${attempt} failed or timed out"
	# Killed apt may leave dpkg half-configured
	sudo -E dpkg --configure -a || true
	sleep 10
done

phase "giving up after ${ATTEMPTS} attempts"
exit 1
