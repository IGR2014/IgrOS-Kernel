#!/bin/sh

make IGROS_ARCH=i386 rebuild
make IGROS_ARCH=i386 deploy
make IGROS_ARCH=x86_64 rebuild
make IGROS_ARCH=x86_64 deploy


