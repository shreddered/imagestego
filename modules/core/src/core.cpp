#include "imagestego/core.hpp"
// third party
#include "MurmurHash3.h"

// intrinsic
#ifdef _MSC_VER
#   include <intrin.h>
#   pragma intrinsic(_BitScanReverse)
#endif

namespace imagestego {

Exception::Exception(const int& _code) noexcept : std::exception(), code(_code) {}

#ifndef _MSC_VER
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
#endif

uint32_t hash(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), IMAGESTEGO_MURMURHASH_SEED, tmp);
    return tmp[0];
}

} // namespace imagestego
