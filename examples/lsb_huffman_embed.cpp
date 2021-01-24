// Example of using LSB embedder with Huffman enthropy coding
// Usage:
//   lsb_huffman_embed <path/to/image> <message> <key> <output>

#include <imagestego/algorithm/lsb.hpp>
#include <imagestego/compression/huffman.hpp>

#include <iostream>

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: lsb <path/to/image> <message> <key> <output>" << std::endl;
        return 1;
    }
    imagestego::LsbEmbedder lsb(new imagestego::HuffmanEncoder);
    lsb.setImage(argv[1]);
    lsb.setMessage(argv[2]);
    lsb.setSecretKey(argv[3]);
    lsb.createStegoContainer(argv[4]);
    return 0;
}
