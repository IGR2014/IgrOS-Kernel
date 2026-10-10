#!/usr/bin/env bash
################################################################
#
#	Boot smoke test: boot kernel ISO in headless QEMU
#
#	Passes when kernel reaches the end of kmain ("Booted successfully")
#	and handles a keyboard interrupt (IDT, PIC, EOI). Fails on triple
#	fault, kernel exception message or timeout.
#
#	Usage: qemu-boot.sh <i386|x86_64> <kernel.iso> [timeout seconds]
#	QEMU binary can be overridden with QEMU environment variable.
#
################################################################

set -u

ARCH="$1"
ISO="$2"
TIMEOUT="${3:-30}"
QEMU="${QEMU:-qemu-system-${ARCH}}"

WORK="$(mktemp -d)"
SERIAL="${WORK}/serial.txt"
DEBUG="${WORK}/qemu.log"
MONITOR="${WORK}/monitor"
mkfifo "${MONITOR}.in" "${MONITOR}.out"
touch "${SERIAL}"

# Wait until serial output contains text (or QEMU exits / timeout)
waitFor() {
	local steps=$((TIMEOUT * 2))
	for ((i = 0; i < steps; i++)); do
		grep -qF "$1" "${SERIAL}" && return 0
		kill -0 "${PID}" 2>/dev/null || return 1
		sleep 0.5
	done
	return 1
}

"${QEMU}" -m 128M -cdrom "${ISO}" -boot d -display none \
	-serial "file:${SERIAL}" -monitor "pipe:${MONITOR}" \
	-no-reboot -d cpu_reset,guest_errors -D "${DEBUG}" &
PID=$!
# Drain monitor output, keep monitor input open
cat "${MONITOR}.out" > /dev/null &
exec 3> "${MONITOR}.in"

RESULT=0
if ! waitFor "Booted successfully"; then
	echo "FAILED: kernel didn't finish booting"
	RESULT=1
else
	# Press and release Enter: kernel prints keyboard IRQ for both
	echo "sendkey ret" >&3
	if ! waitFor "RELEASED"; then
		echo "FAILED: keyboard interrupt not handled"
		RESULT=1
	fi
fi

echo "quit" >&3 2>/dev/null
exec 3>&-
wait "${PID}" 2>/dev/null

if grep -qE "EXCEPTION|Exception:|UNHANDLED" "${SERIAL}"; then
	echo "FAILED: kernel reported exception"
	RESULT=1
fi
if grep -q "Triple fault" "${DEBUG}" 2>/dev/null; then
	echo "FAILED: triple fault"
	RESULT=1
fi

echo "----- serial output (${ARCH})"
tr -d '\r' < "${SERIAL}"
echo "-----"
[ "${RESULT}" -eq 0 ] && echo "BOOT TEST PASSED (${ARCH})" || echo "BOOT TEST FAILED (${ARCH})"

rm -rf "${WORK}"
exit "${RESULT}"
