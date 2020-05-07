#include "imagestego/compression/lzw_encoder.hpp"

#include <iostream>


int main() {
    imagestego::LzwEncoder enc;
    enc.setMessage("abcabca");
    enc.getEncodedMessage();
    return 0;
}
