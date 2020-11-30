[![Build Status](https://travis-ci.com/shreddered/imagestego.svg?branch=develop)](https://travis-ci.com/shreddered/imagestego)
[![Build status](https://ci.appveyor.com/api/projects/status/stugi74mkai6x44q/branch/develop?svg=true)](https://ci.appveyor.com/project/shreddered/imagestego/branch/develop)
[![codecov](https://codecov.io/gh/shreddered/imagestego/branch/develop/graph/badge.svg)](https://codecov.io/gh/shreddered/imagestego)

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
- [Building](#building)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

# Overview

This library consists of components as described below.

- _core_ is the main component of this library, which introduces types/functions/etc needed for other components to work.

- _compression_ component introduces data compression algorithms.

- _lossless_ component contains steganographic schemes with lossless destination.

- _jpeg_ component contains steganographic schemes with JPEG writer.

- _wavelet_ component contains different wavelet transforms.

# Integration

## CMake

### Subdirectory

If you want to embed _imagestego_ into your project, you should use `add_subdirectory()`.
After that `imagestego` interface target should be available:

```cmake
add_executable(foo
    foo.cpp
)

add_subdirectory(path/to/imagestego/directory)

target_link_libraries(foo imagestego)
```

### Find package

If you have _imagestego_ already installed, consider using `find_package()`:

```cmake
find_package(imagestego CONFIG REQUIRED)

add_executable(foo
    foo.cpp
)

target_link_libraries(foo imagestego)
```

# Installing dependencies

This project needs [cmake](https://cmake.org/) to be built from source and [opencv](https://github.com/opencv/opencv) as a dependency.

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

Use [brew](https://brew.sh/):
```bash
$ brew install cmake opencv
```

## Windows

At first you will need [cmake](https://cmake.org/) to build this project.

Grab [opencv release](https://github.com/opencv/opencv/releases/latest) which is suitable for your Visual Studio version.
Then extract it and run `setup_vars_opencv4.cmd`.

Also you will need to set `OPENCV_DIR` environmental variabe to specify path to opencv.
You can open Windows command line and type:
```bat
set "OPENCV_DIR=<path\to\opencv\build>"
```

# Building

First make sure that you have followed all steps presented in [installing depencies](#installing-dependencies) topic.

If you want to build this library, then you have to do the following:
```bash
$ git clone https://github.com/shreddered/imagestego.git
$ cd imagestego/
$ mkdir build/ && cd build/
$ cmake -D CMAKE_INSTALL_PREFIX=<optional: install prefix here> ..
$ make
# if you want to install it
$ sudo make install
```

# Usage

The basic usage is:

```cpp
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
