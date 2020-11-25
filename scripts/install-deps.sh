#!/usr/bin/env bash

# Script for installing dependencies of imagestego on Travis CI
set -e

git clone https://github.com/opencv/opencv.git
mkdir -p opencv/build && pushd opencv/build
cmake -D BUILD_LIST=core,imgcodecs \
 -D CMAKE_BUILD_TYPE=Release \
 -D BUILD_EXAMPLES=OFF \
 -D BUILD_opencv_apps=OFF \
 -D BUILD_DOCS=OFF \
 -D BUILD_PERF_TESTS=OFF \
 -D BUILD_TESTS=OFF \
 -D CMAKE_INSTALL_PREFIX=/usr ..
make -j$(nproc --all)
sudo make install
popd
rm -rf opencv/
