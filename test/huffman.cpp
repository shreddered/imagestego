#include <gtest/gtest.h>

#include <imagestego/compression/huffman_decoder.hpp>
#include <imagestego/compression/huffman_encoder.hpp>


TEST(TestHuffman, HuffmanEncoder) {
    imagestego::HuffmanEncoder encoder("beep boop beer");
    auto tmp = encoder.getEncodedMessage();
    imagestego::BitArray<unsigned char> arr(tmp);
    imagestego::HuffmanDecoder decoder(arr);
    auto str = decoder.getDecodedMessage();
    EXPECT_EQ("beep boop beer", str);
}
