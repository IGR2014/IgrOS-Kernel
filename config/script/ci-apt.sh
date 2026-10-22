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

# Don't rebuild man page index after install (slow on runner images)
sudo rm -f /var/lib/man-db/auto-update

for ((attempt = 1; attempt <= ATTEMPTS; attempt++)); do
	phase "attempt ${attempt}/${ATTEMPTS}: update"
	if timeout --kill-after=10 "${UPDATE_TIMEOUT}" "${APT[@]}" update; then
		phase "attempt ${attempt}/${ATTEMPTS}: install $*"
		if timeout --kill-after=10 "${INSTALL_TIMEOUT}" "${APT[@]}" install -y --no-install-recommends "$@"; then
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
