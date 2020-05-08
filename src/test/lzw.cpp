#include "imagestego/compression/lzw_encoder.hpp"
#include "imagestego/compression/lzw_decoder.hpp"

#include <iostream>


int main(int argc, char** argv) {
    std::string s(argv[1]);
    imagestego::LzwEncoder enc;
    enc.setMessage(s);
    auto arr = enc.getEncodedMessage();
    imagestego::LzwDecoder dec(arr);
    std::cout << dec.getDecodedMessage() << std::endl;
    return 0;
}
