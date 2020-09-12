#include "imagestego/core.hpp"
// third party
#include "MurmurHash3.h"


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
#if defined(__clang__) || defined(__GNUC__)
    return value ? 31 - __builtin_clz(value) : 0;
#elif defined(_MSC_VER)
    unsigned long result = 0;
    BitScanReverse(&result, value);
    return result;
#else
    uint8_t res = 0;
    while (value >>= 1)
        ++res;
    return res;
#endif
}

uint32_t hash(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), IMAGESTEGO_MURMURHASH_SEED, tmp);
    return tmp[0];
}

} // namespace imagestego
