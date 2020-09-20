#include "imagestego/compression/lzw_encoder.hpp"
// c++ headers
#include <iostream>


namespace imagestego {

LzwEncoder::LzwEncoder() noexcept : Dictionary() {}

LzwEncoder::LzwEncoder(const std::string& str) noexcept : Dictionary(), msg(str) {}

LzwEncoder::LzwEncoder(std::string&& str) noexcept : Dictionary(), msg(str) {}

void LzwEncoder::setMessage(const std::string& str) noexcept {
    msg = str;
    encodedMsg.clear();
}

void LzwEncoder::setMessage(std::string&& str) noexcept {
    msg = str;
    encodedMsg.clear();
}

BitArray<> LzwEncoder::getEncodedMessage() {
    if (encodedMsg.empty() && msg.size() != 1) {
        encode();
    }
    return encodedMsg;
}

void LzwEncoder::encode() {
    StringElement s;
    uint8_t currentBitsPerBlock = 8;
    std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
    encodedMsg.pushBack(maxBits, 4); 
    for (std::size_t i = 0; i != msg.size(); ++i) {
        s.value = msg[i];
        int index = Dictionary::search(s);
        if (index != -1) {
            s.prefixIndex = index;
        }
        else {
            encodedMsg.pushBack(s.prefixIndex, currentBitsPerBlock);
            s.prefixIndex = s.value;
            if (Dictionary::size() > currentMaxDictionarySize) { 
                if (currentBitsPerBlock == maxBits) {
                    currentBitsPerBlock = 8;
                    Dictionary::clear();
                }
                else
                    ++currentBitsPerBlock;
                currentMaxDictionarySize = (1 << currentBitsPerBlock);
            }
        }
    }
    encodedMsg.pushBack(s.prefixIndex, currentBitsPerBlock);
}

} // namespace imagestego
