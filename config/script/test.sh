#!/usr/bin/env bash
################################################################
#
#	Run host unit tests and QEMU boot test of kernel built by build.sh
#
#	Usage: test.sh <i386|x86_64> [clang++|g++] [debug|release]
#
################################################################

set -eu

ARCH="${1:?usage: test.sh <i386|x86_64> [clang++|g++] [debug|release]}"
COMPILER="${2:-clang++}"
TYPE="${3:-debug}"

case "${TYPE}" in
	debug)		TYPE_DIR="Debug"	;;
	release)	TYPE_DIR="Release"	;;
	*)		echo "Unknown build type: ${TYPE}" >&2; exit 1	;;
esac

cd "$(dirname "$0")/../.."

# Unit tests (host compiler, kernel architecture code paths)
cmake -S tests -B "build/tests-${ARCH}" -DCMAKE_CXX_COMPILER="${COMPILER}" -DIGROS_TEST_ARCH="${ARCH}"
cmake --build "build/tests-${ARCH}" --parallel
ctest --test-dir "build/tests-${ARCH}" --output-on-failure --no-tests=error

# Boot test
bash tests/boot/qemu-boot.sh "${ARCH}" "install/${TYPE_DIR}/config-linux-${COMPILER}-${ARCH}-${TYPE}/kernel.iso"
