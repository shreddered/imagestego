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
#include <algorithm>
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

TEST(Core, BitArray4) {
    BitArray arr("10001010"),
             arr1("1010101010");
    EXPECT_TRUE(arr[0] == arr1[0]);
    EXPECT_TRUE(arr[0] != arr[1]);
}

TEST(Core, BitArrayComparison) {
    BitArray arr = std::string("1001"), tmp = std::string("1100");
    EXPECT_EQ(arr[0], tmp[0]);
    arr[1] = tmp[1];
    arr[3] = tmp[3];
    EXPECT_TRUE(arr == tmp);
}

TEST(Core, BitArrayIterator) {
    BitArray arr = std::string("11010100101001001010");
    std::ostringstream ss;
    for (auto it = arr.begin(); it != arr.end(); ++it)
        ss << *it;
    EXPECT_EQ(ss.str(), arr.toString());
    std::string s = ss.str();
    std::ostringstream().swap(ss);
    for (auto it = arr.begin(); it != arr.end(); it++)
        ss << *it;
    EXPECT_EQ(ss.str(), s);
    auto it = arr.end();
    --it;
    EXPECT_TRUE(!*it);
    it--;
    EXPECT_TRUE(*it);
    BitArray tmp("1010101010100101010100100101010010100001001010100101010010101000010101");
    std::ostringstream().swap(ss);
    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        ss << *it;
    }
    EXPECT_EQ(ss.str(), tmp.toString());
}

TEST(Core, BitArrayCopyMove) {
    BitArray arr("10001010100100101010100101001010101");
    BitArray copy = arr;
    EXPECT_TRUE(copy == arr);
    arr = copy;
    EXPECT_TRUE(copy == arr);
    BitArray arr1 = std::move(copy);
    EXPECT_TRUE(arr1 == arr);
    BitArray arr3;
    arr3 = std::move(arr1);
    EXPECT_TRUE(arr == arr3);
}

TEST(Core, BitArrayPut) {
    BitArray arr(32);
    BitArray tmp = BitArray::fromByteString(std::string(4, '\0'));
    EXPECT_TRUE(arr == tmp);
    BitArray arr1;
    arr1.put(1 << 5, 32);
    tmp = BitArray("00000000000000000000000000100000");
    EXPECT_TRUE(arr1 == tmp);
    BitArray arr2;
    arr2.put(64u, 32);
    tmp = BitArray("00000000000000000000000001000000");
    EXPECT_TRUE(arr2 == tmp);
}
