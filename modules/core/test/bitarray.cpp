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

// imagestego
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/intrinsic.hpp"
// c++ headers
#include <bitset>
#include <sstream>
// gtest
#include <gtest/gtest.h>

using imagestego::BitArray;

std::bitset<32> reverse32(const std::bitset<32>& set) {
    const imagestego::size_t n = static_cast<imagestego::size_t>(set.to_ulong());
    return std::bitset<32>(imagestego::bswap(n));
}


TEST(Core, BitArray1) { 
    uint32_t temp = 53;
    char buf[4];
    memcpy(buf, &temp, sizeof(temp));
    BitArray arr = BitArray::fromByteString(std::string(buf, 4));
    std::ostringstream ss;
    ss << arr;
    EXPECT_EQ(reverse32(std::bitset<32>(temp)).to_string(), ss.str());
    EXPECT_EQ(ss.str(), arr.toString());
    std::ostringstream().swap(ss);
    BitArray copy = arr;
    ss << copy;
    EXPECT_EQ(ss.str(), reverse32(std::bitset<32>(temp)).to_string());
}

TEST(Core, BitArray2) {
    std::string msg = "asdasdkk";
    BitArray arr = BitArray::fromByteString(msg);
    EXPECT_EQ(msg, arr.toByteString());
    arr.clear();
    EXPECT_TRUE(arr.empty());
}

TEST(Core, BitArray3) {
    uint32_t temp = 53;
    char buf[4];
    memcpy(buf, &temp, sizeof(temp));
    BitArray arr = BitArray::fromByteString(std::string(buf, 4));
    std::string s = arr.toString();
    arr.pushBack(1); s.push_back('1');
    EXPECT_EQ(arr.toString(), s);
}
