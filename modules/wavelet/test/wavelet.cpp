/*
 * this file is a part of imagestego library.
 *
 * copyright (c) 2020-2021 dmitry kalinin <x.shreddered.x@gmail.com>
 *
 * permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "software"), to deal
 * in the software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the software is
 * furnished to do so, subject to the following conditions:
 *
 * the above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the software.
 *
 * the software is provided "as is", without warranty of any kind, express or
 * implied, including but not limited to the warranties of merchantability,
 * fitness for a particular purpose and noninfringement. in no event shall the
 * authors or copyright holders be liable for any claim, damages or other
 * liability, whether in an action of contract, tort or otherwise, arising from,
 * out of or in connection with the software or the use or other dealings in the
 * software.
 */

// imagestego headers
#include "imagestego/algorithm/wavelet.hpp"
#include "imagestego/compression/huffman.hpp"
#include "imagestego/wavelet/haar.hpp"
// c++ headers
#include <iostream>
// gtest
#include <gtest/gtest.h>

TEST(Wavelet, WaveletEmbedder) {
    imagestego::WaveletEmbedder emb(new imagestego::HaarWavelet);
    emb.setMessage("test message");
    emb.setSecretKey("key");
    emb.setImage("test.jpg");
    emb.createStegoContainer("wavelet.png");

    imagestego::WaveletExtracter ext(new imagestego::HaarWavelet);
    ext.setImage("wavelet.png");
    ext.setSecretKey("key");
    EXPECT_EQ(ext.extractMessage(), "test message");
}

TEST(Wavelet, WaveletEmbedderHuffman) {
    imagestego::WaveletEmbedder emb(new imagestego::HaarWavelet,
                                    new imagestego::HuffmanEncoder);
    emb.setMessage("asad dsda sssa sadsa");
    emb.setSecretKey("key");
    emb.setImage("test.jpg");
    emb.createStegoContainer("wavelet_huffman.png");

    imagestego::WaveletExtracter ext(new imagestego::HaarWavelet,
                                     new imagestego::HuffmanDecoder);
    ext.setImage("wavelet_huffman.png");
    ext.setSecretKey("key");
    std::cout << ext.extractMessage() << std::endl;
}
