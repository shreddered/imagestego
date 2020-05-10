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

    // DWT embedding with no encoder 
    imagestego::DwtEmbedder<void> embeddingTest4("in.jpg", "out4.png");
    embeddingTest4.setMessage(msg);
    embeddingTest4.setSecretKey("asda");
    embeddingTest4.createStegoContainer();
    // DWT extracting with no decoder
    imagestego::DwtExtracter<void> extractingTest4("out4.png");
    extractingTest4.setSecretKey("asda");
    assert(msg == extractingTest4.extractMessage());

    // DWT embedding with Huffman encoder 
    imagestego::DwtEmbedder<imagestego::HuffmanEncoder> embeddingTest5("in.jpg", "out5.png");
    embeddingTest5.setMessage(msg);
    embeddingTest5.setSecretKey("asda");
    embeddingTest5.createStegoContainer();
    // DWT extracting with Huffman decoder
    imagestego::DwtExtracter<imagestego::HuffmanDecoder> extractingTest5("out5.png");
    extractingTest5.setSecretKey("asda");
    assert(msg == extractingTest5.extractMessage());
    // DWT embedding with LZW encoder 
    imagestego::DwtEmbedder<imagestego::LzwEncoder> embeddingTest6("in.jpg", "out6.png");
    embeddingTest6.setMessage(msg);
    embeddingTest6.setSecretKey("asda");
    embeddingTest6.createStegoContainer();
    // DWT extracting with LZW decoder
    imagestego::DwtExtracter<imagestego::LzwDecoder> extractingTest6("out6.png");
    extractingTest6.setSecretKey("asda");
    assert(msg == extractingTest6.extractMessage());
    
    // JPEG LSB embedding with Huffman encoder 
    imagestego::JpegLsbEmbedder<imagestego::HuffmanEncoder> embeddingTest7("in.jpg", "out7.jpg");
    embeddingTest7.setMessage(msg);
    embeddingTest7.setSecretKey("asda");
    embeddingTest7.createStegoContainer();
    // DWT extracting with Huffman decoder
    imagestego::JpegLsbExtracter<imagestego::HuffmanDecoder> extractingTest7("out7.jpg");
    extractingTest7.setSecretKey("asda");
    assert(msg == extractingTest7.extractMessage());
    return 0;
}
