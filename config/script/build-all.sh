#!/bin/sh

cmake -DIGROS_ARCH=i386 -DCMAKE_TOOLCHAIN_FILE=config/cmake/clang++-i386.cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B ./build/i386
cmake --build ./build/i386 --target all --parallel

cmake -DIGROS_ARCH=x86_64 -DCMAKE_TOOLCHAIN_FILE=config/cmake/clang++-x86_64.cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B ./build/x86_64
cmake --build ./build/x86_64 --target all --parallel
