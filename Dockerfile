# Ubuntu 24.04
FROM ubuntu:24.04

# Build args
ARG IGROS_ARCH
ARG IGROS_COMPILER

# Install build dependencies
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
		lld \
		mtools \
		ninja-build \
		xorriso && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

# Copy sources to docker
WORKDIR /home/igros/kernel
# Copy source code
COPY . /home/igros/kernel

# Restore cache (assumes mounted volume or separate caching mechanism)
VOLUME ["/home/igros/kernel/ccache"]

# Configure kernel
RUN \
	cmake --preset="config-linux-${IGROS_COMPILER}-${IGROS_ARCH}-debug"

# Build kernel
RUN \
	cmake --build --preset="build-linux-${IGROS_COMPILER}-${IGROS_ARCH}-debug" --target all --parallel

# Expose build
VOLUME ["/home/igros/kernel/build"]

# Install kernel
RUN \
	cmake --build --preset="build-linux-${IGROS_COMPILER}-${IGROS_ARCH}-debug" --target install

# Expose artifacts as a volume
VOLUME ["/home/igros/kernel/install"]

# CCache statistics
RUN \
	ccache -sv

# Default command
ENTRYPOINT ["tail", "-f", "/dev/null"]
