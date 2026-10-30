#!/usr/bin/env bash
################################################################
#
#	Boot smoke test: boot kernel ISO in headless BOCHS
#
#	Passes when kernel reaches the end of kmain ("Booted successfully")
#	with working UART (BOCHS transmits with real character timing,
#	unlike QEMU). Fails on BOCHS panic, triple fault, kernel exception,
#	self-test or driver failure message or timeout.
#	Keyboard is not tested: BOCHS has no monitor to send keys.
#
#	Usage: bochs-boot.sh <i386|x86_64> <kernel.iso> [timeout seconds]
#	BOCHS binary can be overridden with BOCHS environment variable,
#	BIOS directory - with BXSHARE. Without DISPLAY runs under xvfb-run.
#
################################################################

set -u

ARCH="$1"
ISO="$2"
TIMEOUT="${3:-180}"
BOCHS="${BOCHS:-bochs}"
BIOS_DIR="${BXSHARE:-/usr/share/bochs}"

WORK="$(mktemp -d)"
SERIAL="${WORK}/serial.txt"
DEBUG="${WORK}/bochs.log"
CONFIG="${WORK}/bochsrc"
touch "${SERIAL}"

# Legacy BIOS: rombios32 of BIOS-bochs-latest crashes (triple fault) in Bochs 2.8 of Ubuntu 26.04;
# no time sync: emulation runs as fast as possible
cat << EOF > "${CONFIG}"
romimage: file=${BIOS_DIR}/BIOS-bochs-legacy
megs: 128
ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14
ata0-master: type=cdrom, path="${ISO}", status=inserted
boot: cdrom
cpu: reset_on_triple_fault=0
clock: sync=none
display_library: x
com1: enabled=1, mode=file, dev=${SERIAL}
log: ${DEBUG}
panic: action=fatal
EOF

# X display for BOCHS window (virtual one on headless machines)
LAUNCH=()
[[ -n "${DISPLAY:-}" ]] || LAUNCH=(xvfb-run -a)

# Own process group, so BOCHS (and Xvfb) can be stopped together;
# debugger builds wait for a command first: continue
echo "c" | setsid "${LAUNCH[@]}" "${BOCHS}" -q -f "${CONFIG}" > "${WORK}/bochs.out" 2>&1 &
PID=$!

# Wait until serial output contains text (or BOCHS exits / timeout)
wait_for() {
	local text="$1"
	local steps=$((TIMEOUT * 2))
	for ((i = 0; i < steps; i++)); do
		grep -qF "${text}" "${SERIAL}" && return 0
		kill -0 "${PID}" 2>/dev/null || return 1
		sleep 0.5
	done
	return 1
}

RESULT=0
if ! wait_for "Booted successfully"; then
	echo "FAILED: kernel didn't finish booting"
	RESULT=1
fi

kill -- "-${PID}" 2>/dev/null
wait "${PID}" 2>/dev/null

if ! grep -qE "^Driver UART:.*OK" "${SERIAL}"; then
	echo "FAILED: UART not initialized"
	RESULT=1
fi
if grep -qE "EXCEPTION|Exception:|UNHANDLED" "${SERIAL}"; then
	echo "FAILED: kernel reported exception"
	RESULT=1
fi
if grep -q "FAILED" "${SERIAL}"; then
	echo "FAILED: kernel self-test failed"
	RESULT=1
fi
if grep -qE "^Driver .*ERROR" "${SERIAL}"; then
	echo "FAILED: driver init failed"
	RESULT=1
fi
if grep -qE ">>PANIC<<|exception with no resolution" "${DEBUG}" 2>/dev/null; then
	echo "FAILED: BOCHS panic (triple fault?)"
	grep -m3 ">>PANIC<<" "${DEBUG}"
	RESULT=1
fi

echo "----- serial output (${ARCH})"
tr -d '\r' < "${SERIAL}"
echo "-----"
if [[ "${RESULT}" -ne 0 ]]; then
	echo "----- BOCHS output"
	tail -20 "${WORK}/bochs.out"
	echo "-----"
fi
[[ "${RESULT}" -eq 0 ]] && echo "BOOT TEST PASSED (${ARCH}, BOCHS)" || echo "BOOT TEST FAILED (${ARCH}, BOCHS)"

rm -rf "${WORK}"
exit "${RESULT}"
