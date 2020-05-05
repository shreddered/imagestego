# Overview

## LSB algorithm

Algorithm converts message represented as string to bit array and then prepends length of it.
After that, classical embedding is performed.

Several options are introduced:

* `imagestego::LsbOptions::silly`
In this case, consequential embedding will be performed.

* `imagestego::LsbOptions::randomBits`
In this case, points for embedding will be chosen randomly.
For points generation balanced binary tree is used.

Also, key string needed. It is used to seed PRNG. 
Key string will be converted to bit array too. The embedding process is as follows:
1. The i-th bit of key is xored with LSB of pixel's red value.
2. If xor above is equal to 0, current bit of message will be embedded into LSB of pixel's green value.
Otherwise,  current bit of message will be embedded into LSB of pixel's blue value.

**TODO**: migrate from C random API.

* `imagestego::LsbOptions::usePlusMinusOne`

This case includes `imagestego::LsbOptions::randomBits`.
Every pixel value before changing LSB will be incremented/decremented (chose randomly).
This helps resisting stegoanalysis.

As for JPEG, embedding process performed after DCT coefficients quantization.

## DWT-based algorithm

This algorithm uses [Integer Wavelet Transform (IWT)](https://stackoverflow.com/questions/15802827/how-can-dwt-be-used-in-lsb-substitution-steganography) to embed secret data.

The embedding process is as follows:
1. 2-dimensional IWT performed on the blue channel of an image 
2. Embedding performed in LSB of 2-dimensional highpass submatrix values.
3. Inverse IWT performed on new matrix.
4. Merge channels and write image.

**TODO**: add key-dependent cropping.
