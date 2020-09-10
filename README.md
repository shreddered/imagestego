[![Build Status](https://travis-ci.com/shreddered/imagestego.svg?branch=master)](https://travis-ci.com/shreddered/imagestego)

# imagestego 

Image Steganography library written in _c++11_.
Dependencies - [_libopencv_](https://github.com/opencv/opencv) and [_libjpeg_](http://www.ijg.org/).

# Quick overview


In this library several steganographic algorithms are introduced:

* LSB algorithm.

This algorithm uses LSB technique to embed secret message.
LSB embedding in JPEG applied after quantization of DCT coefficients.

* DWT-based algorithm (lossless formats only).

This scheme uses [Integer Wavelet Transform](https://stackoverflow.com/questions/15802827/how-can-dwt-be-used-in-lsb-substitution-steganography) to embed secret message.

* F3 Algorithm

If you want an detailed overview of algorithms please refer to [OVERVIEW.md](https://github.com/shreddered/Image-Steganography/blob/master/OVERVIEW.md).

Also there is a support for data comression:

* [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding)

* [LZW](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)

# Building

First of all, you need to install _libopencv_, _libjpeg_ and _cmake_.

## Debian
```bash
$ sudo apt-get update
$ sudo apt-get install libjpeg-dev cmake libopencv-dev
```
## Arch Linux
```bash
$ sudo pacman -Syu
$ sudo pacman -S cmake libjpeg-dev libopencv-dev
```
## macOS
Use [_brew_](https://brew.sh/) for installing dependencies.
```bash
$ brew install cmake libjpeg opencv
```
After that, just run
```bash
$ cmake -Bbuild
$ cd build/
$ make -j4
$ sudo make install
```
_libimagestego_ will be compiled and placed into `/usr/local/lib` directory.

## Windows

Please refer to [_cmake_ downloads](https://cmake.org/download/), [_libjpeg_](http://www.ijg.org/)
and [_opencv_ releases](https://opencv.org/releases/) for complete guidelines.

# Usage

For a high-level functions you can just `#include <imagestego/imagestego.hpp>` file,
which has two functions - `embedSecretMessage` and `extractSecretMesage`:

```c++
// imagestego/imagestego.hpp

namespace imagestego {

void embedSecretMessage(AbstractStegoEmbeder* embedder, const std::string& input, 
                        const std::string& output, const std::string& key);

std::string extractSecretMessage(AbstractStegoExtracter* extracter, const std::string& key);

}
```

Abstract classes is defined like this

```c++
// imagestego/core.hpp

namespace imagestego {

class IMAGESTEGO_EXPORTS AbstractStegoEmbedder {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setOutputName(const std::string& filename) = 0;
    virtual void setMessage(const std::string& msg) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual void createStegoContainer() const = 0;
};

class IMAGESTEGO_EXPORTS AbstractStegoExtracter {
public:
    virtual void setImage(const std::string& imageName) = 0;
    virtual void setSecretKey(const std::string& key) = 0;
    virtual std::string extractMessage() = 0;
};

}
```

Alternatively, you can `#include <imagestego/algorithms.hpp>` file to manage classes on your own
or you can even create another class template to use with LZW/Huffman coders.

For more details see project wiki (it's being updated).

# License

This software is distibuted under GNU GPLv3. For more details see [LICENSE.txt](https://github.com/shreddered/Image-Steganography/blob/master/LICENSE.txt).

# Further development
- [x] Implement LZW data compression algorithm
- [ ] Implement at least one more stego algorithm
