#!/bin/bash -ex

mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=./mingw.cmake .. -G Ninja
cmake --build . -j12 -v
