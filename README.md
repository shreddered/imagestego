[![Build Status](https://travis-ci.com/shreddered/imagestego.svg?branch=master)](https://travis-ci.com/shreddered/imagestego)
[![Build status](https://ci.appveyor.com/api/projects/status/stugi74mkai6x44q/branch/master?svg=true)](https://ci.appveyor.com/project/shreddered/imagestego/branch/master)
[![codecov](https://codecov.io/gh/shreddered/imagestego/branch/master/graph/badge.svg)](https://codecov.io/gh/shreddered/imagestego)

# imagestego

Image Steganography library written in _c++11_.
Dependencies - [_opencv_](https://github.com/opencv/opencv) and [_libjpeg_](http://www.ijg.org/).

# Contents

- [Overview](#overview)
- [Integration](#integration)
    * [CMake](#cmake)
- [Installing dependencies](#installing-dependencies)
    * [Debian](#debian)
    * [Arch Linux](#arch-linux)
    * [macOS](#mac-os)
    * [Windows](#windows)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

# Overview

This library has several modules.

The _core_ module is the main module of this library, which introduces a lot of types/functions/etc needed for other modules to work.

The _compression_ module introduces data compression algorithms.

The _lossless_ module contains steganographic schemes with lossless destination.

The _jpeg_ module contains steganographic schemes with JPEG writer.

The _wavelet_ module contains different wavelet transforms.

# Integration

## CMake

### Subdirectory

If you want to embed _imagestego_ into your project, just use `add_subdirectory()`.
After that `imagestego` interface target should be available:

```cmake
add_executable(foo
    foo.cpp
)

add_subdirectory(path/to/imagestego/directory)

target_link_libraries(foo imagestego)
```

### Find package

If you have _imagestego_ already installed, you can simply add it with `find_package()`:

```cmake
find_package(imagestego CONFIG REQUIRED)

add_executable(foo
    foo.cpp
)

target_link_libraries(foo imagestego)
```

# Installing dependencies

## Debian

```bash
$ sudo apt update
$ sudo apt install cmake libopencv-dev
```
## Arch Linux

```bash
$ sudo pacman -S cmake opencv
```

## macOS

Use [brew](https://brew.sh/).
```bash
$ brew install cmake opencv
```

## Windows

Select [opencv release](https://github.com/opencv/opencv/releases) suitable for your visual studio version.

Also you will need to set `OPENCV_DIR` environmental variabe to specify path to opencv.
```bat
> set "OPENCV_DIR=<path\to\opencv\build>"
```

# Building

If you want to build this library, then you have to do several steps.
```bash
$ git clone https://github.com/shreddered/imagestego.git
$ cd imagestego/
$ mkdir build/ && cd build/
$ cmake -D CMAKE_INSTALL_PREFIX=<optional: install prefix here>..
$ make
# if you want to install it
$ sudo make install
```

# Usage

The basic usage is:

```cpp
// ...
// imagestego
#include <imagestego/imagestego.hpp>

int main(int argc, char** argv) {
    imagestego::embedSecretMessage(new imagestego::LsbEmbedder, "in.png",
                                   "out.png", "message", "key");
    return 0;
}
```

# Contributing

Please see [Contributing](https://github.com/shreddered/imagestego/wiki/Contributing) wiki page for more information.

# License

This software is distibuted under GNU GPLv3. For more details see [LICENSE.txt](LICENSE.txt).

# Further development

- [ ] Wavelet module
