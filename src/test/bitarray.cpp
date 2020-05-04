#include <utils/bitarray.hpp>

#include <bitset>
#include <cassert>
#include <iostream>
#include <sstream>


int main() {
    imagestego::BitArray<> arr = imagestego::BitArray<>::fromInt(12);
    std::stringstream ss;
    ss << arr;
    assert(ss.str() == std::bitset<32>(12).to_string());
    std::stringstream().swap(ss); // clear ss
    arr.clear();
    imagestego::BitArray<> arr1 = imagestego::BitArray<>::fromInt(12313);
    ss << arr1;
    std::cout << ss.str() << std::endl;
    std::cout << std::bitset<32>(12313) << std::endl;
    assert(ss.str() == std::bitset<32>(12313).to_string());
    return 0;
}
