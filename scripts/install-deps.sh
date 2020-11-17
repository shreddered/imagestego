#!/usr/bin/env sh

# Script for installing dependencies of imagestego on Travis CI

 git clone https://github.com/opencv/opencv.git
 cd opencv
 mkdir build
 cd build
 cmake -D BUILD_LIST=core,imgcodecs \
     -D CMAKE_BUILD_TYPE=Release \
     -D BUILD_EXAMPLES=OFF \
     -D BUILD_opencv_apps=OFF \
     -D BUILD_DOCS=OFF \
     -D BUILD_PERF_TESTS=OFF \
     -D BUILD_TESTS=OFF \
     -D CMAKE_INSTALL_PREFIX=/usr/local/ ..
 make -j$(nproc)
 sudo make install
 if [ "${TRAVIS_OS_NAME}" = "linux" ]; then sudo sh -c 'echo "/usr/local/lib/" > /etc/ld.so.conf.d/opencv.conf'; fi
 if [ "${TRAVIS_OS_NAME}" = "linux" ]; then sudo ldconfig; fi
 cd ../../
