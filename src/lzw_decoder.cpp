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
}

} // namespace imagestego
