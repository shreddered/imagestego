#include <iostream>

#include <huffman_decoder.hpp>
#include <huffman_encoder.hpp>


int main(int argc, char** argv) {
    if (argc < 3)
        return -1;
    if (std::string(argv[1]) == "encode") {
        HuffmanEncoder encoder(argv[2]);
        std::cout << encoder.getEncodedMessage() << std::endl;
    }
    else if (std::string(argv[1]) == "decode") {
        BitArray<unsigned char> arr(argv[2], BitArray<unsigned char>::bitString);
        HuffmanDecoder decoder(arr);
        std::cout << decoder.getDecodedMessage() << std::endl;
    }
    return 0;
}
