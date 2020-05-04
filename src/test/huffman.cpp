#include <cassert>

#include <huffman_decoder.hpp>
#include <huffman_encoder.hpp>


int main(int argc, char** argv) {
    imagestego::HuffmanEncoder encoder("beep boop beer");
    auto tmp = encoder.getEncodedMessage();
    imagestego::BitArray<unsigned char> arr(tmp);
    imagestego::HuffmanDecoder decoder(arr);
    auto str = decoder.getDecodedMessage();
    assert("beep boop beer" == str);
    return 0;
}
