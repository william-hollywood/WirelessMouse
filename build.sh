#!/bin/bash -ex

mkdir -p build
cd build
cmake .. -G Ninja
cmake --build . -j"$(nproc)" -v "${@}"
