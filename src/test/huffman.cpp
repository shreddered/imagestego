#include <cassert>

#include <imagestego/compression/huffman_decoder.hpp>
#include <imagestego/compression/huffman_encoder.hpp>


int main(int argc, char** argv) {
    imagestego::HuffmanEncoder encoder("beep boop beer");
    auto tmp = encoder.getEncodedMessage();
    imagestego::BitArray<unsigned char> arr(tmp);
    imagestego::HuffmanDecoder decoder(arr);
    auto str = decoder.getDecodedMessage();
    assert("beep boop beer" == str);
    return 0;
}
