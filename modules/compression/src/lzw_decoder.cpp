#include "imagestego/compression/lzw_decoder.hpp"

#include <iostream>


template<typename Int>
int read(const imagestego::BitArray<Int>& arr, std::size_t& i, uint8_t bits) {
    int block = 0;
    std::size_t it;
    for (it = i; it != i + bits; ++it)
        block |= arr[it] << (bits - 1 - (it - i)); 
    i = it;
    return block;
}

namespace imagestego {

LzwDecoder::LzwDecoder() noexcept : Dictionary() {}

LzwDecoder::LzwDecoder(const BitArray<>& arr) noexcept : Dictionary(), msg(arr) {}

std::string LzwDecoder::getDecodedMessage() {
    if (decodedMsg.empty()) {
        decode();
    }
    return decodedMsg;
}

void LzwDecoder::decode() {
    // reading first 4 bits    
    std::size_t i = 0;
    uint8_t maxBits = read(msg, i, 4);
    while(i < msg.size()) {
        Dictionary::clear();
        uint8_t currentBitsPerBlock = 8;
        std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock);
        std::size_t code = read(msg, i, currentBitsPerBlock);
        decodedMsg += code;
        std::size_t oldCode = code;
        while (1) {
            if (i >= msg.size())
                break;
            if (size() == currentMaxDictionarySize) {
                if (currentBitsPerBlock == maxBits)
                    break;
                ++currentBitsPerBlock;
                currentMaxDictionarySize = (1 << currentBitsPerBlock);
            }
            code = read(msg, i, currentBitsPerBlock);
            std::string tmp;
            if (code < Dictionary::size()) {
                tmp = Dictionary::at(code);
                decodedMsg += tmp;
            }
            else {
                tmp = Dictionary::at(oldCode);
                decodedMsg += tmp + tmp[0];
            }
            Dictionary::add(tmp[0], oldCode);
            oldCode = code;
        }
    }
}

} // namespace imagestego
