#!/usr/bin/env bash

# Script for building imagestego on Travis CI
set -e

mkdir -p build && cd build
cmake -D CMAKE_BUILD_TYPE=Release \
    -D BUILD_SHARED_LIBS=ON \
    -D IMAGESTEGO_BUILD_TESTS=ON \
    -D IMAGESTEGO_COVERAGE=ON \
    -D IMAGESTEGO_MODULES=core,compression,lossless,wavelet ..
make -j$(nproc)
cd ..
