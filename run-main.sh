#!/usr/bin/env sh
# if [ ! -d "build" ]; then
# 	mkdir build
# fi
# cmake -S . -B build -G"Unix Makefiles"
# cd build
# make all
scripts/run_bazelisk.py build //main:j7
bazel-bin/main/j7 file.j7
