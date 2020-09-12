#include "imagestego/utils/jpeg_image.hpp"
#include "imagestego/algorithms/jpeg_lsb.hpp"
#include "imagestego/compression.hpp"

#include <iostream>


int main() {
    imagestego::JpegLsbEmbedder<imagestego::LzwEncoder> emb("in.jpg", "out.jpg");
    emb.setMessage("ASS WE CAN");
    emb.setSecretKey("key");
    emb.createStegoContainer();
    imagestego::JpegLsbExtracter<imagestego::LzwDecoder> ext("out.jpg");
    ext.setSecretKey("key");
    std::cout << ext.extractMessage() << std::endl;
    return 0;
}
