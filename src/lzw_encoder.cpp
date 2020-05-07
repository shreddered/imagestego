#include "imagestego/compression/lzw_encoder.hpp"


namespace imagestego {

LzwEncoder::LzwEncoder() noexcept : Dictionary() {}

LzwEncoder::LzwEncoder(const std::string& str) noexcept : Dictionary(), msg(str) {}

LzwEncoder::LzwEncoder(std::string&& str) noexcept : Dictionary(), msg(str) {}

void LzwEncoder::setMessage(const std::string& str) noexcept {
    msg = str;
}

void LzwEncoder::setMessage(std::string&& str) noexcept {
    msg = str;
}

} // namespace imagestego
