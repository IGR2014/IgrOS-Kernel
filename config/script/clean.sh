#!/usr/bin/env bash
################################################################
#
#	Remove build and install directories
#
#	Usage: clean.sh
#
################################################################

set -eu

cd "$(dirname "$0")/../.."

rm -rf build install
