# Image-Steganography

Image Steganography library written in _c++11_.
Dependencies - [_libopencv_](https://github.com/opencv/opencv) и [_libjpeg_](http://www.ijg.org/).

# Quick overview
In this library several steganographic algorithms are introduced:
* LSB algorithm.

This algorithm uses LSB technique to embed secret message.
LSB embedding in JPEG applied after quantization of DCT coefficients.

* DWT-based algorithm (lossless formats only).

This scheme uses [Integer Wavelet Transform](https://stackoverflow.com/questions/15802827/how-can-dwt-be-used-in-lsb-substitution-steganography) to embed secret message.

If you want detailed overview of algorithms please refer to [OVERVIEW.md](https://github.com/shreddered/Image-Steganography/blob/master/OVERVIEW.md)

Also there is a support for data comression:
* [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding)

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
$ brew install cmake libjpeg libopencv-dev
```
After that, just run
```bash
$ cmake -Bbuild
$ cd build/
$ make -j4
```
`libimagestego.so` will be compiled and placed into `lib/` directory.

## Windows
Please refer to [_cmake_ downloads](https://cmake.org/download/), [_libjpeg_](http://www.ijg.org/)
and [_opencv_ releases](https://opencv.org/releases/) for complete guidelines.
# Further development
- [ ] Implement LZW data compression algorithm
- [ ] Implement at least one more stego algoritm
- [ ] Make DWT-based algorithm format-independent
