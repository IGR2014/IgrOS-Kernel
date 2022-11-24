#!/bin/sh

cmake -DCMAKE_TOOLCHAIN_FILE=config/cmake/clang++-x86_64.cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B ./build/x86_64
cmake --build ./build/x86_64 --target test

