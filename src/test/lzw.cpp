#include "imagestego/compression/lzw_encoder.hpp"

#include <iostream>


int main() {
    std::string s = "abcabca";
    imagestego::LzwEncoder enc;
    enc.setMessage(s);
    std::cout << (s.size() << 3) << ' ' << enc.getEncodedMessage() << std::endl;
    return 0;
}
