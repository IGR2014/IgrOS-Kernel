# Ubuntu 26.04
FROM ubuntu:26.04 AS toolchain

# Run as root
USER root

# Toolchain only: sources are mounted at runtime (see docker-compose.yaml),
# so code changes don't require rebuilding the image
# (versions follow the Ubuntu 26.04 archive, like CI runners - not pinned)
# hadolint ignore=DL3008
RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends \
		ccache \
		clang \
		cmake \
		doxygen \
		g++-multilib \
		gcc-multilib \
		git \
		graphviz \
		grub-common \
		grub-pc-bin \
		lld \
		mtools \
		ninja-build \
		xorriso && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/* && \
	git config --system --add safe.directory /home/igros/kernel

# Run as unprivileged user by default (docker-compose overrides it with host UID/GID)
USER ubuntu

# Mounted sources
WORKDIR /home/igros/kernel

# Compiler cache inside mounted sources (ignored by git)
ENV CCACHE_DIR=/home/igros/kernel/.ccache

# Configure, build and install kernel
# Usage: docker run ... <arch> <compiler> [debug|release]
ENTRYPOINT ["/bin/sh", "-c", "set -e; preset=\"linux-$1-$0-${2:-debug}\"; cmake --preset=\"config-$preset\"; cmake --build --preset=\"build-$preset\" --target all --parallel; cmake --build --preset=\"build-$preset\" --target install; ccache -s"]


# CodeQL bundle for analysis stage: Docker fetches it over HTTPS and verifies the
# pinned checksum, unpacked here so the archive doesn't stay in an analysis layer
FROM ubuntu:26.04 AS codeql

# CodeQL release (C/C++ bundle: CLI and query packs)
ARG CODEQL_VERSION=2.27.2
ARG CODEQL_SHA256=638338a6489c4229c06012239f570dd76992b4ed862c1a7922d46496622a72ca
ADD --checksum=sha256:${CODEQL_SHA256} \
	"https://github.com/github/codeql-action/releases/download/codeql-bundle-v${CODEQL_VERSION}/codeql-bundle-cpp-linux64.tar.zst" \
	/tmp/codeql.tar.zst

# Bundle files are 0600 of the release builder: keep them readable for any (host) UID
# hadolint ignore=DL3008
RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends zstd && \
	tar --zstd --no-same-owner -xf /tmp/codeql.tar.zst -C /opt && \
	chmod -R a+rX /opt/codeql


# Analysis (see config/script/analysis.sh): toolchain + static analyzers,
# coverage and QEMU for the UBSan boot check
FROM toolchain AS analysis

# Run as root
USER root

# ClangTidy from apt.llvm.org: newer checks than Ubuntu archive has
# (same version as in Analysis workflow)
ARG LLVM_VERSION=23
COPY config/apt/llvm.asc config/script/llvm-apt.sh /tmp/llvm/

# Analysis tools (certificates first: LLVM repository is HTTPS)
# hadolint ignore=DL3008
RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends ca-certificates && \
	bash /tmp/llvm/llvm-apt.sh "${LLVM_VERSION}" /tmp/llvm/llvm.asc && \
	apt-get update && \
	apt-get install -y --no-install-recommends "clang-tidy-${LLVM_VERSION}" \
		cppcheck \
		gcovr \
		python3 \
		qemu-system-x86 && \
	ln -s "/usr/bin/clang-tidy-${LLVM_VERSION}" /usr/local/bin/clang-tidy && \
	ln -s "/usr/bin/run-clang-tidy-${LLVM_VERSION}" /usr/local/bin/run-clang-tidy && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/* /tmp/llvm

# CodeQL CLI with C/C++ query packs (unpacked bundle from codeql stage)
COPY --from=codeql /opt/codeql /opt/codeql
RUN ln -s /opt/codeql/codeql /usr/local/bin/codeql

# Run as unprivileged user by default (docker-compose overrides it with host UID/GID)
USER ubuntu

# Analyze all (or given) configs, report in build/analysis/index.html
# Usage: docker run ... [<clang++|g++>:<i386|x86_64> ...]
ENTRYPOINT ["bash", "config/script/analysis.sh"]


# VS Code Dev Container (see .devcontainer/devcontainer.json):
# toolchain + debugging tools
FROM toolchain AS dev

# Run as root
USER root

# Devcontainer addons
# hadolint ignore=DL3008
RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends \
		bochs \
		bochs-x \
		bochsbios \
		gdb \
		qemu-system-x86 \
		vgabios && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

# Run as unprivileged user by default (docker-compose overrides it with host UID/GID)
USER ubuntu

