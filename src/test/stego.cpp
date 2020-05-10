#include "imagestego/algorithms.hpp"
#include "imagestego/compression.hpp"
// c++ headers
#include <cassert>


int main() {
    std::string msg = "asdasdsdfjjahdajdajfgbsdasdasdasdadfshfbdjanbudvasgdkasbdnmadhj";

    // LSB embedding using Huffman encoder
    imagestego::LsbEmbedder<imagestego::HuffmanEncoder> embeddingTest1("in.jpg", "out1.png",
            imagestego::LsbOptions::randomBits);
    embeddingTest1.setMessage(msg);
    embeddingTest1.setSecretKey("asda");
    embeddingTest1.createStegoContainer();
    // LSB extracting using Huffman decoder
    imagestego::LsbExtracter<imagestego::HuffmanDecoder> extractingTest1("out1.png",
            imagestego::LsbOptions::randomBits);
    extractingTest1.setSecretKey("asda");
    assert(msg == extractingTest1.extractMessage());

    // LSB embedding using LZW encoder
    imagestego::LsbEmbedder<imagestego::LzwEncoder> embeddingTest2("in.jpg", "out2.png",
            imagestego::LsbOptions::randomBits);
    embeddingTest2.setMessage(msg);
    embeddingTest2.setSecretKey("poasdi");
    embeddingTest2.createStegoContainer();
    // LSB extracting using LZW decoder
    imagestego::LsbExtracter<imagestego::LzwDecoder> extractingTest2("out2.png",
            imagestego::LsbOptions::randomBits);
    extractingTest2.setSecretKey("poasdi");
    assert(msg == extractingTest2.extractMessage());

    // LSB embedding with no encoder 
    imagestego::LsbEmbedder<void> embeddingTest3("in.jpg", "out3.png",
            imagestego::LsbOptions::randomBits);
    embeddingTest3.setMessage(msg);
    embeddingTest3.setSecretKey("asda");
    embeddingTest3.createStegoContainer();
    // LSB extracting with no decoder
    imagestego::LsbExtracter<void> extractingTest3("out3.png",
            imagestego::LsbOptions::randomBits);
    extractingTest3.setSecretKey("asda");
    assert(msg == extractingTest3.extractMessage());
    return 0;
}
