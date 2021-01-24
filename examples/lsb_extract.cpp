// Example of LSB-based extracting
// Usage:
//   lsb_extract <path/to/image> <key>

#include "imagestego/algorithm/lsb.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: lsb <path/to/image> <key>" << std::endl;
        return 1;
    }
    imagestego::LsbExtracter lsb;
    lsb.setImage(argv[1]);
    lsb.setSecretKey(argv[2]);
    std::cout << lsb.extractMessage() << std::endl;
    return 0;
}
