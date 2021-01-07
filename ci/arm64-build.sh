#!/usr/bin/env bash

set -e

apt update -q -y
apt install -q -y cmake gcc g++ libopencv-dev

cmake -S . -B build \
    -D CMAKE_BUILD_TYPE=Release \
    -D IMAGESTEGO_BUILD_TESTS=ON \
    -D IMAGESTEGO_BUILD_PERF_TESTS=OFF \
cmake --build build -j 8
cd build
cp ../test/test.jpg .
ctest --output-on-failure
