#include "imagestego/compression/lzw_encoder.hpp"
#include "imagestego/compression/lzw_decoder.hpp"

#include <cassert>
#include <iostream>


int main(int argc, char** argv) {
    std::string s = "asdasdajhfahnsahsfhasghfuadhashdjahwakshdahduahs    ahsdahsdakdjajdkjj opi";
    imagestego::LzwEncoder enc;
    enc.setMessage(s);
    auto arr = enc.getEncodedMessage();
    imagestego::LzwDecoder dec(arr);
    auto s1 = dec.getDecodedMessage();
    std::cout << dec.getDecodedMessage() << std::endl;
    assert(s1 == s);
    return 0;
}
