#!/usr/bin/env bash

# Script for installing dependencies of imagestego on Travis CI
set -e

if [ ! -d $TRAVIS_BUILD_DIR/opencv/build ]; then
    git clone https://github.com/opencv/opencv.git
    mkdir -p opencv/build && cd opencv/build
    cmake -D BUILD_LIST=core,imgcodecs \
     -D CMAKE_BUILD_TYPE=Release \
     -D BUILD_EXAMPLES=OFF \
     -D BUILD_opencv_apps=OFF \
     -D BUILD_DOCS=OFF \
     -D BUILD_PERF_TESTS=OFF \
     -D BUILD_TESTS=OFF \
     -D CMAKE_INSTALL_PREFIX=/usr/local/ ..
    make -j$(nproc)
else
    cd $TRAVIS_BUILD_DIR/opencv/build
fi
sudo make install
if [ "${TRAVIS_OS_NAME}" = "linux" ]; then sudo sh -c 'echo "/usr/local/lib/" > /etc/ld.so.conf.d/opencv.conf'; fi
if [ "${TRAVIS_OS_NAME}" = "linux" ]; then sudo ldconfig; fi
cd ../..
