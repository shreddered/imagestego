/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

// imagestego headers
#include "imagestego/core/exception.hpp"
#include "imagestego/core/interfaces.hpp"
#include <imagestego/algorithm/lsb.hpp>
#include <imagestego/compression/huffman_encoder.hpp>
#include <imagestego/compression/huffman_decoder.hpp>
// gtest headers
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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

TEST(Lossless, LsbType) {
    LsbEmbedder emb;
    EXPECT_EQ(emb.getAlgorithm(), imagestego::Algorithm::Lsb);

    LsbExtracter ext;
    EXPECT_EQ(ext.getAlgorithm(), imagestego::Algorithm::Lsb);
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
