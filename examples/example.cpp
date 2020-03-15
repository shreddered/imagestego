#include <imagestego.hpp>
#include <bitarray.hpp>
#include <iostream>


int main(int argc, char** agrv) {
    int a = 1;
    inc(a);
    //std::cout << "Hello, world!" << a <<std::endl;
    auto arr = BitArray<uint16_t>("10");
    std::cout << arr.size() << std::endl;
    return 0;
}
