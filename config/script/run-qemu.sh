#!/usr/bin/env bash
################################################################
#
#	Run kernel ISO built by build.sh in QEMU (serial output to terminal)
#
#	Usage: run-qemu.sh <i386|x86_64> [clang++|g++] [debug|release] [QEMU arguments...]
#
################################################################

set -eu

ARCH="${1:?usage: run-qemu.sh <i386|x86_64> [clang++|g++] [debug|release] [QEMU arguments...]}"
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
[ -f "${ISO}" ] || { echo "No ${ISO}: run config/script/build.sh ${ARCH} ${COMPILER} ${TYPE} first" >&2; exit 1; }

exec "${QEMU:-qemu-system-${ARCH}}" -m 128M -cdrom "${ISO}" -boot d -serial stdio "$@"
