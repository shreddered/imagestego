#include "imagestego/compression/lzw_decoder.hpp"


template<typename It>
int read(It it, uint8_t bits) {
    std::size_t block = 0;
    for (uint8_t i = 0; i != bits; ++i) {
        block |= *(it + i) << (bits - i - 1);
    }
    return block;
}

template<typename Int>
int read(const imagestego::BitArray<Int>& arr, std::size_t& i, uint8_t bits) {
    int block = 0;
    for (auto it = i; i != it + bits; ++i)
        block |= arr[i] << (bits - 1 - i + it); 
    return block;
}

namespace imagestego {

LzwDecoder::LzwDecoder() noexcept : Dictionary() {}

LzwDecoder::LzwDecoder(const BitArray<>& arr) noexcept : Dictionary(), msg(arr) {}

void LzwDecoder::setMessage(const BitArray<>& arr) noexcept {
    msg = arr;
    decodedMsg.clear();
}

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
    while(i != msg.size()) {
        Dictionary::clear();
        uint8_t currentBitsPerBlock = 8;
        std::size_t currentMaxDictionarySize = (1 << currentBitsPerBlock) - 1;
        std::size_t code = read(msg, i, currentBitsPerBlock);
        decodedMsg += code;
        std::size_t oldCode = code;
        while (Dictionary::size() != currentMaxDictionarySize && currentBitsPerBlock != maxBits && i != msg.size()) {
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
            if (size() == currentMaxDictionarySize && currentBitsPerBlock != maxBits) {
                ++currentBitsPerBlock;
                currentMaxDictionarySize = (1 << currentBitsPerBlock) - 1;
            }
            oldCode = code;
        }
    }
}

} // namespace imagestego
