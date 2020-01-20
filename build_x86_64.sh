#!/bin/sh

cd arch
make IGROS_ARCH=x86_64 rebuild
make IGROS_ARCH=x86_64 deploy
cd ..
