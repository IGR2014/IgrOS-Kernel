#!/bin/sh

cd arch
make IGROS_ARCH=i386 rebuild
make IGROS_ARCH=i386 deploy
cd ..
