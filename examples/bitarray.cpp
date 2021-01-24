// Example of using imagestego::BitArray class

#include <imagestego/core.hpp>

#include <iostream>
#include <string>

using imagestego::BitArray;

int main() {
    std::string s = "hello world";
    // BitArray can be created from string
    BitArray arr = BitArray::fromByteString(s);
    std::cout << arr << std::endl;

    // Also BitArray can be created from string with binary digits
    // Binary representation of "imagestego" string
    arr = BitArray("0110100101101101011000010110011101100101011100"
                   "1101110100011001010110011101101111"); 
    // Outputs "imagestego"
    std::cout << arr.toByteString() << std::endl;
    // Append 1 to array
    arr.pushBack(1);
    std::cout << arr[arr.size() - 1] << std::endl;
    
    // BitArray can be created from 32-bit integer like std::bitset
    arr = BitArray::fromInt(32);
    // Outputs "00000000000000000000000000100000"
    std::cout << arr << std::endl;
    // BitArray can also be converted to 32-bit integer
    // Outputs 32
    std::cout << arr.toInt() << std::endl;
    return 0;
}
