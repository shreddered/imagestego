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
#include "imagestego/core/bitarray.hpp"
#include <imagestego/compression/lzw_decoder.hpp>
#include <imagestego/compression/lzw_encoder.hpp>
// gtest
#include <gtest/gtest.h>

using imagestego::BitArray;


TEST(Compression, LzwEncoding) {
    imagestego::LzwEncoder enc("asdasdasdad");
    auto arr = enc.getEncodedMessage();
    imagestego::LzwDecoder dec(arr);
    EXPECT_EQ(dec.getDecodedMessage(), "asdasdasdad");
}
