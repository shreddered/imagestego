[![Build Status](https://travis-ci.com/shreddered/imagestego.svg?branch=master)](https://travis-ci.com/shreddered/imagestego)
[![Build status](https://ci.appveyor.com/api/projects/status/stugi74mkai6x44q/branch/master?svg=true)](https://ci.appveyor.com/project/shreddered/imagestego/branch/master)
[![codecov](https://codecov.io/gh/shreddered/imagestego/branch/master/graph/badge.svg)](https://codecov.io/gh/shreddered/imagestego)

# imagestego 

Image Steganography library written in _c++11_.
Dependencies - [_opencv_](https://github.com/opencv/opencv) and [_libjpeg_](http://www.ijg.org/).

- [Overview](#overview)
- [Integration](#integration)
- [License](#license)

# Overview

In this library several steganographic algorithms are introduced:

- LSB algorithm

This algorithm uses LSB technique to embed secret message.
LSB embedding in JPEG applied after quantization of DCT coefficients.

- DWT-based algorithm (lossless formats only)

This scheme uses [Integer Wavelet Transform](https://stackoverflow.com/questions/15802827/how-can-dwt-be-used-in-lsb-substitution-steganography) to embed secret message.

- F3 Algorithm

If you want an detailed overview of algorithms please refer to [OVERVIEW.md](https://github.com/shreddered/Image-Steganography/blob/master/OVERVIEW.md).

Also there is a support for data comression:

- [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding)

- [LZW](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)

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

# License

This software is distibuted under GNU GPLv3. For more details see [LICENSE.txt](LICENSE.txt).

# Further development
- [x] Implement LZW data compression algorithm
- [ ] Implement at least one more stego algorithm
