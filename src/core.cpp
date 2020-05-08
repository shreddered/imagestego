#include "imagestego/core.hpp"


namespace imagestego {

Exception::Exception(const int& _code) noexcept : std::exception(), code(_code) {}

const char* Exception::what() const noexcept {
    switch(code) {
        case Codes::NoSuchFile:
            return "No such file";
        case Codes::BigMessageSize:
            return "Cannot embed message: image is too small";
        case Codes::NoKeyFound:
            return "No key found";
        case Codes::InternalError:
            return "Internal error";
        case Codes::UnknownLsbMode:
            return "Unknown LSB mode";
        case Codes::NotJpegClass:
            return "Class 'LsbEmbedder' doesn't support JPEG pictures. Use 'JpegLsbEmbedder' instead";
        default:
            return "Unknown Error";
    }
}

uint8_t log2(unsigned int value) noexcept {
    uint8_t res = 0;
    while (value >>= 1)
        ++res;
    return res;
}

} // namespace imagestego
