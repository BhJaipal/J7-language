#!/usr/bin/env sh
if [ ! -d "build" ]; then
	mkdir build
fi
cmake -S . -B build -G"Unix Makefiles"
cd build
make all
