/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// imagestego headers
#include "imagestego/core/exception.hpp"
#include "imagestego/core/interfaces.hpp"
#include <imagestego/algorithm/lsb.hpp>
#include <imagestego/compression/huffman_decoder.hpp>
#include <imagestego/compression/huffman_encoder.hpp>
// gtest headers
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;

using namespace imagestego;

class LsbEmbedderMock : public LsbEmbedder {
public:
    explicit LsbEmbedderMock() noexcept : LsbEmbedder() {}
    MOCK_METHOD1(setImage, void(const std::string& src));
    MOCK_METHOD1(setSecretKey, void(const std::string& key));
    MOCK_METHOD1(setMessage, void(const std::string& msg));
    MOCK_METHOD1(createStegoContainer, void(const std::string& dst));
}; // class LsbEmbedderMock

TEST(Lossless, LsbEmbedderMock) {
    LsbEmbedderMock emb;
    EXPECT_CALL(emb, setSecretKey(_)).Times(1);
    EXPECT_CALL(emb, setImage(_)).Times(1);
    EXPECT_CALL(emb, setMessage(_)).Times(1);
    EXPECT_CALL(emb, createStegoContainer(_)).Times(1);
    emb.setSecretKey("key");
    emb.setImage("rare-combinaties.jpg");
    emb.setMessage("message!");
    emb.createStegoContainer("out.png");
}

TEST(Lossless, LsbEmbedder) {
    LsbEmbedder emb;
    emb.setImage("test.jpg");
    emb.setMessage("message!");
    emb.setSecretKey("key");
    emb.createStegoContainer("out.png");

    LsbExtracter ext;
    ext.setImage("out.png");
    ext.setSecretKey("key");
    EXPECT_EQ("message!", ext.extractMessage());
}

TEST(Lossless, LsbExceptions) {
    LsbEmbedder emb;
    emb.setImage("test.jpg");
    emb.setMessage("message!");
    EXPECT_THROW(emb.createStegoContainer("foo.png"), imagestego::Exception);

    LsbExtracter ext;
    ext.setImage("out.png");
    EXPECT_THROW(ext.extractMessage(), imagestego::Exception);
}

TEST(Lossless, LsbHuffman) {
    LsbEmbedder emb(new HuffmanEncoder);
    emb.setImage("test.jpg");
    emb.setMessage("asdasfnsfjhasjdhhjasgdasdasdasdasdasdaaasdasd");
    emb.setSecretKey("key");
    emb.createStegoContainer("out1.png");

    LsbExtracter ext(new HuffmanDecoder);
    ext.setImage("out1.png");
    ext.setSecretKey("key");
    EXPECT_EQ("asdasfnsfjhasjdhhjasgdasdasdasdasdasdaaasdasd", ext.extractMessage());
}
