#include <imagestego/utils/bitarray.hpp>

#include <bitset>
#include <cassert>
#include <iostream>
#include <sstream>


template<typename Int>
int read(const imagestego::BitArray<Int>& arr, std::size_t& i, uint8_t bits) {
    int block = 0;
    for (auto it = i; i != it + bits; ++i)
        block |= arr[i] << (bits - 1 - i + it);
    return block;
}

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

    imagestego::BitArray<> arr2;
    arr2.pushBack(12, 4);
    arr2.pushBack(0);
    arr2.pushBack(0);
    std::size_t i = 0;
    std::cout << read(arr2, i, 4) << std::endl;
    imagestego::BitArray<uint32_t> arr3 = imagestego::BitArray<uint32_t>::fromInt(57);
    std::cout << arr3 << std::endl;
    return 0;
}
