// imagestego
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <bitset>
#include <sstream>
// gtest
#include <gtest/gtest.h>

using imagestego::BitArray;


TEST(Core, BitArray1) { 
    uint32_t temp = 53;
    char buf[4];
    memcpy(buf, &temp, sizeof(temp));
    BitArray arr = BitArray::fromByteString(std::string(buf, 4));
    std::ostringstream ss;
    ss << arr;
    EXPECT_EQ(std::bitset<32>(temp).to_string(), ss.str());
    EXPECT_EQ(ss.str(), arr.toString());
    std::ostringstream().swap(ss);
    BitArray copy = arr;
    ss << copy;
    EXPECT_EQ(ss.str(), std::bitset<32>(temp).to_string());
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
