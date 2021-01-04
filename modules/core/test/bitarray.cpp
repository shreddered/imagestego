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
    const std::size_t n = static_cast<std::size_t>(set.to_ulong());
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
    BitArray copy = arr; // NOLINT: needed for testing
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
    arr.pushBack(1);
    s.push_back('1');
    EXPECT_EQ(arr.toString(), s);
}

TEST(Core, BitArray4) {
    BitArray arr("10001010"), arr1("1010101010");
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
    BitArray tmp(
        "1010101010100101010100100101010010100001001010100101010010101000010101");
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

TEST(Core, BitArrayFromInt) {
    BitArray arr = BitArray::fromInt(32), tmp("00000000000000000000000000100000");
    EXPECT_TRUE(arr == tmp);
}

TEST(Core, BitArrayToByteString) {
    BitArray arr = BitArray::fromByteString("asdss");
    std::string s = arr.toByteString();
    EXPECT_EQ(s, std::string("asdss"));
}

TEST(Core, BitArrayPushFront) {
    BitArray arr, tmp("00000000000000000000000000100000");
    arr.pushFront(32);
    EXPECT_TRUE(arr == tmp);
}

TEST(Core, BitArrayToInt) {
    BitArray arr;
    arr.pushFront(32);
    EXPECT_EQ(arr.toInt(), 32);
}
