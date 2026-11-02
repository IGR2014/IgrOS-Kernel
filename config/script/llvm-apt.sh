#!/usr/bin/env bash
#
#	Add apt.llvm.org repository of given LLVM version (newer clang-tidy than
#	Ubuntu archive has), signed by key kept in config/apt/llvm.asc
#	(usage: llvm-apt.sh <version> [key file], as root; run apt-get update after)
#
#	File:	llvm-apt.sh
#	Date:	02 Nov 2026
#
#	Copyright (c) 2017 - 2026, Igor Baklykov
#	All rights reserved.
#
#


set -eu

LLVM_VERSION="${1:?Usage: llvm-apt.sh <version> [key file]}"
KEY="${2:-$(dirname "$0")/../apt/llvm.asc}"

# Ubuntu release codename (repository suite name; also sets VERSION, hence LLVM_VERSION)
# shellcheck source=/dev/null
. /etc/os-release

install -D -m 644 "${KEY}" /etc/apt/keyrings/llvm.asc
cat > /etc/apt/sources.list.d/llvm.sources <<EOF
Types: deb
URIs: https://apt.llvm.org/${VERSION_CODENAME}/
Suites: llvm-toolchain-${VERSION_CODENAME}-${LLVM_VERSION}
Components: main
Signed-By: /etc/apt/keyrings/llvm.asc
EOF
