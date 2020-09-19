// imagestego
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <bitset>
#include <sstream>
// gtest
#include <gtest/gtest.h>


TEST(Core, BitArray) { 
    uint32_t temp = 53;
    char buf[4];
    memcpy(buf, &temp, sizeof(temp));
    std::stringstream ss;
    imagestego::BitArray arr = imagestego::BitArray::fromByteString(std::string(buf, 4));
    ss << arr;
    EXPECT_EQ(std::bitset<32>(temp).to_string(), ss.str());
}
