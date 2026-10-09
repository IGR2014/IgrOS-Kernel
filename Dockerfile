# Ubuntu 24.04
FROM ubuntu:24.04

# Toolchain only: sources are mounted at runtime (see docker-compose.yaml),
# so code changes don't require rebuilding the image
RUN \
	apt-get update && \
	apt-get install -y --no-install-recommends \
		git \
		ccache \
		clang \
		cmake \
		doxygen \
		graphviz \
		g++-multilib \
		gcc-multilib \
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
