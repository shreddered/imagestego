#include "imagestego/compression/lzw_encoder.hpp"


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
    // TODO: add Dictionary::reset()
    StringElement s;
    std::vector<int> encoded;
    for (std::size_t i = 0; i != msg.size(); ++i) {
        s.value = msg[i];
        int index = Dictionary::search(s);
        if (index != -1) {
            s.prefixIndex = index;
        }
        else {
            encoded.push_back(s.prefixIndex);
            s.prefixIndex = s.value;
        }
    }
    encoded.push_back(s.prefixIndex);
    auto bitsPerUnit = log2(Dictionary::size()) + 1;
}

} // namespace imagestego
