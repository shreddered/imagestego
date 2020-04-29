#include <cassert>

#include <huffman_decoder.hpp>
#include <huffman_encoder.hpp>


int main(int argc, char** argv) {
    HuffmanEncoder encoder("beep boop beer");
    auto tmp = encoder.getEncodedMessage();
    BitArray<unsigned char> arr(tmp);
    HuffmanDecoder decoder(arr);
    auto str = decoder.getDecodedMessage();
    assert("beep boop beer" == str);
    return 0;
}
