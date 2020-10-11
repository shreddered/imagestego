[![Build Status](https://travis-ci.com/shreddered/imagestego.svg?branch=master)](https://travis-ci.com/shreddered/imagestego)
[![Build status](https://ci.appveyor.com/api/projects/status/stugi74mkai6x44q/branch/master?svg=true)](https://ci.appveyor.com/project/shreddered/imagestego/branch/master)
[![codecov](https://codecov.io/gh/shreddered/imagestego/branch/master/graph/badge.svg)](https://codecov.io/gh/shreddered/imagestego)

# imagestego

Image Steganography library written in _c++11_.
Dependencies - [_opencv_](https://github.com/opencv/opencv) and [_libjpeg_](http://www.ijg.org/).

- [Overview](#overview)
- [Integration](#integration)
- [Build at your own](#build-at-your-own)
- [Contributing](#contributing)
- [License](#license)

# Overview

This library has several modules.

The _core_ module is the main module of this library, which introduces a lot of types/functions/etc needed for other modules to work.

The _compression_ module introduces data compression algorithms.

The _lossless_ module contains steganographic schemes with lossless destination.

The _jpeg_ module contains steganographic schemes with JPEG writer.


# Integration

## CMake

If you want to embed _imagestego_ into your project, just use `add_subdirectory()`
```cmake
add_executable(foo
    foo.cpp
)

add_subdirectory(path/to/imagestego/directory)

target_link_libraries(foo imagestego)
```

# Build at your own

If you want to build this library, then you have to do several steps.
```bash
$ git clone https://github.com/shreddered/imagestego.git
$ cd imagestego/
$ mkdir build/ && cd build/
$ cmake ..
$ make
# if you want to install it
$ sudo make install
```

# Contributing

Please see [Contributing](https://github.com/shreddered/imagestego/wiki/Contributing) wiki page for more information.

# License

This software is distibuted under GNU GPLv3. For more details see [LICENSE.txt](LICENSE.txt).

# Further development
- [ ] Wavelet module
