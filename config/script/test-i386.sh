#!/bin/sh

cmake -DCMAKE_TOOLCHAIN_FILE=config/cmake/clang++-i386.cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B ./build/i386
cmake --build ./build/i386 --target test

