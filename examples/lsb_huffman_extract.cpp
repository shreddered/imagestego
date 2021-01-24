// Example of using LSB extracter with Huffman enthropy coding
// Usage:
//   lsb_huffman_extract <path/to/image> <key>

#include <imagestego/algorithm/lsb.hpp>
#include <imagestego/compression/huffman.hpp>

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: lsb <path/to/image> <key>" << std::endl;
        return 1;
    }
    imagestego::LsbExtracter lsb(new imagestego::HuffmanDecoder);
    lsb.setImage(argv[1]);
    lsb.setSecretKey(argv[2]);
    std::cout << lsb.extractMessage() << std::endl;
    return 0;
}
