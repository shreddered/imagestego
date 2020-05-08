#include "imagestego/compression/lzw_encoder.hpp"
#include "imagestego/compression/lzw_decoder.hpp"

#include <fstream>
#include <iostream>


int main(int argc, char** argv) {
    std::ifstream in("bitarray.hpp");
    std::ofstream out("bitarray1.hpp");
    std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    imagestego::LzwEncoder enc;
    enc.setMessage(s);
    auto arr = enc.getEncodedMessage();
    imagestego::LzwDecoder dec(arr);
    out << dec.getDecodedMessage() << std::endl;
    return 0;
}
