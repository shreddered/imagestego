# Image-Steganography

[![Build Status](https://travis-ci.com/shreddered/Image-Steganography.svg?branch=master)](https://travis-ci.com/shreddered/Image-Steganography)

Image Steganography library written in _c++11_.
Dependencies - [_libopencv_](https://github.com/opencv/opencv) and [_libjpeg_](http://www.ijg.org/).

# Quick overview

In this library several steganographic algorithms are introduced:

* LSB algorithm.

This algorithm uses LSB technique to embed secret message.
LSB embedding in JPEG applied after quantization of DCT coefficients.

* DWT-based algorithm (lossless formats only).

This scheme uses [Integer Wavelet Transform](https://stackoverflow.com/questions/15802827/how-can-dwt-be-used-in-lsb-substitution-steganography) to embed secret message.

If you want an detailed overview of algorithms please refer to [OVERVIEW.md](https://github.com/shreddered/Image-Steganography/blob/master/OVERVIEW.md).

Also there is a support for data comression:

* [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding)

* [LZW](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Welch)

# Building

First of all, you need to install _libopenc-dev_, _libjpeg-dev_ and _cmake_ (needed for building).

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

## Command line tool

There is a command line tool called `imagestego` which comes with the library.
The usage is pretty simple. See `imagestego -h` for details.

## Library

For a high-level functions you can just `#include <imagestego/imagestego.hpp>` file,
which has two functions - `embedSecretMessage` and `extractSecretMesage`.

Alternatively, you can `#include <imagestego/algorithms.hpp>` file to manage classes on your own
or you can even create another class template to use with LZW/Huffman coders.

# Further development
- [x] Implement LZW data compression algorithm
- [ ] Implement at least one more stego algorithm
