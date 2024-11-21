#!/usr/bin/env bash

if [ $1 = "-S" ]; then
	cd .. && meson setup --wipe build/
elif [ $1 = "S" ]; then
	meson setup --wipe build/
elif [ $1 = "B" ]; then
	ninja
fi
