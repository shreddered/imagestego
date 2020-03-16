#include <imagestego.hpp>
#include <utils/bitarray.hpp>

#include <bitset>
#include <iostream>


int main(int argc, char** agrv) {
    //std::cout << "Hello, world!" << a <<std::endl;
    auto arr = BitArray<uint16_t>("Hello world");
    std::cout << arr << std::endl;
    std::cout << arr.toString() << std::endl;
    return 0;
}
