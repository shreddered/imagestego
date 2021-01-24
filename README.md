[![Build status](https://ci.appveyor.com/api/projects/status/stugi74mkai6x44q/branch/master?svg=true)](https://ci.appveyor.com/project/shreddered/imagestego/branch/master)
[![Ubuntu](https://github.com/shreddered/imagestego/workflows/Ubuntu/badge.svg)](https://github.com/shreddered/imagestego/actions?query=workflow%3AUbuntu)
[![macOS](https://github.com/shreddered/imagestego/workflows/macOS/badge.svg)](https://github.com/shreddered/imagestego/actions?query=workflow%3AmacOS)
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

- _wavelet_ component contains different wavelet transforms.

# Integration

## CMake

### Subdirectory

If you want to embed _imagestego_ into your project, you should use `add_subdirectory()`.
After that `imagestego` interface target will be available:

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

Go to [cmake downloads](https://cmake.org/download/) to install CMake.

Grab [opencv release](https://github.com/opencv/opencv/releases/latest) which is suitable for your Visual Studio version.
Then extract it and run `setup_vars_opencv4.cmd`.

Also you will need to set `OPENCV_DIR` environmental variabe to specify path to opencv.
You can open Windows command line and type:
```bat
set "OPENCV_DIR=<path\to\opencv\build>"
```

# Building

First make sure that you have followed all steps presented in [installing depencies](#installing-dependencies) topic.

If you want to build _imagestego_ from source, then you have to do the following:
```bash
$ git clone https://github.com/shreddered/imagestego.git
$ cd imagestego/
$ mkdir build/ && cd build/
$ cmake -D CMAKE_INSTALL_PREFIX=<optional: install prefix here> ..
$ make
# if you want to install it
$ sudo make install
```

If you are interested in building tests, then you should do the following:
```bash
$ mkdir build/ && cd build/
$ cmake -D CMAKE_INSTALL_PREFIX=<optional: install prefix here> ..
$ make
$ cp ../test/test.jpg . # copy test data
$ ctest --verbose
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

More examples are coming soon!

# Contributing

Please see [Contributing](https://github.com/shreddered/imagestego/wiki/Contributing) wiki page for more information.

# License

This software is distibuted under MIT License. For more details see [LICENSE](LICENSE).

# Further development

- [x] JPEG module
- [ ] Vectorize inverse haar wavelet
- [ ] Implement more steganographic schemes
- [ ] Implement more wavelet schemes
