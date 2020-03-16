#include <imagestego.hpp>
#include <utils/bitarray.hpp>
#include <iostream>


int main(int argc, char** agrv) {
    int a = 1;
    inc(a);
    //std::cout << "Hello, world!" << a <<std::endl;
    auto arr = BitArray<uint16_t>("110");
    std::cout << arr.size() << std::endl;
    for (int i = 0; i < arr.size(); ++i)
        std::cout << arr[i];
    return 0;
}
