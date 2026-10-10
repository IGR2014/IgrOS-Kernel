#!/usr/bin/env bash
################################################################
#
#	Configure, build and install kernel (bootable ISO) with CMake presets
#
#	Usage: build.sh <i386|x86_64> [clang++|g++] [debug|release]
#	Result: install/<Debug|Release>/config-linux-<compiler>-<arch>-<type>/kernel.iso
#
################################################################

set -eu

ARCH="${1:?usage: build.sh <i386|x86_64> [clang++|g++] [debug|release]}"
COMPILER="${2:-clang++}"
TYPE="${3:-debug}"
PRESET="linux-${COMPILER}-${ARCH}-${TYPE}"

cd "$(dirname "$0")/../.."

cmake --preset "config-${PRESET}"
cmake --build --preset "build-${PRESET}" --target all --parallel
cmake --build --preset "build-${PRESET}" --target install
