// imagestego
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <bitset>
#include <sstream>
// gtest
#include <gtest/gtest.h>

using imagestego::BitArray;


TEST(Core, BitArray) { 
    uint32_t temp = 53;
    char buf[4];
    memcpy(buf, &temp, sizeof(temp));
    std::ostringstream ss;
    BitArray arr = BitArray::fromByteString(std::string(buf, 4));
    ss << arr;
    EXPECT_EQ(std::bitset<32>(temp).to_string(), ss.str());
    std::ostringstream().swap(ss);
    BitArray copy = arr;
    ss << copy;
    EXPECT_EQ(ss.str(), std::bitset<32>(temp).to_string());
}
