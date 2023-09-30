#!/usr/bin/bash
# sudo apt install ninja-build
rm -rf ./target
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B ./target
#cmake -B ./target --fresh
cmake --build ./target --target jkill -j 18