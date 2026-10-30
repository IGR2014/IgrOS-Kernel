#!/usr/bin/env bash
################################################################
#
#	Run kernel ISO built by build.sh in BOCHS
#
#	Usage: run-bochs.sh <i386|x86_64> [clang++|g++] [debug|release] [BOCHS arguments...]
#
################################################################

set -eu

ARCH="${1:?usage: run-bochs.sh <i386|x86_64> [clang++|g++] [debug|release] [BOCHS arguments...]}"
COMPILER="${2:-clang++}"
TYPE="${3:-debug}"
shift $(( $# < 3 ? $# : 3 ))

case "${TYPE}" in
	debug)		TYPE_DIR="Debug"	;;
	release)	TYPE_DIR="Release"	;;
	*)		echo "Unknown build type: ${TYPE}" >&2; exit 1	;;
esac

cd "$(dirname "$0")/../.."

ISO="install/${TYPE_DIR}/config-linux-${COMPILER}-${ARCH}-${TYPE}/kernel.iso"
[[ -f "${ISO}" ]] || { echo "No ${ISO}: run config/script/build.sh ${ARCH} ${COMPILER} ${TYPE} first" >&2; exit 1; }

CONFIG="install/${TYPE_DIR}/config-linux-${COMPILER}-${ARCH}-${TYPE}/bochs-config"

# Legacy BIOS: rombios32 of BIOS-bochs-latest crashes (triple fault) in Bochs 2.8 of Ubuntu 26.04
BIOS_DIR="${BXSHARE:-/usr/share/bochs}"

cat << EOF > "${CONFIG}"
romimage: file=${BIOS_DIR}/BIOS-bochs-legacy
ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14
ata0-master: type=cdrom, path="./${ISO}", status=inserted
boot: cdrom
cpu: reset_on_triple_fault=0
display_library: sdl2

EOF

exec "${BOCHS:-bochs}" -f "${CONFIG}" "$@"
