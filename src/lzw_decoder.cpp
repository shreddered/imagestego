#include "imagestego/compression/lzw_decoder.hpp"


namespace imagestego {

LzwDecoder::LzwDecoder() noexcept : Dictionary() {}

LzwDecoder::LzwDecoder(const BitArray<>& arr) noexcept : Dictionary(), msg(arr) {}

void setMessage(const BitArray<>& arr) noexcept {
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
    StringElement s;
    uint8_t bitsPerBlock = 0;
    for (char i = 0; i != 4; ++i) {
        bitsPerBlock |= msg[i] << (3 - i);
    }
    for (std::size_t i = 4; i != msg.size(); i += bitsPerBlock) {
        int block = 0;
        for (char j = 0; j != bitsPerBlock; ++j) {
            block |= msg[i + j] << (bitsPerBlock - 1 - j);
        }
        if (block < Dictionary::size())
    }
}

} // namespace imagestego
