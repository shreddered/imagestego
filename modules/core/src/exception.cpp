#include "imagestego/core/exception.hpp"


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

} // namespace imagestego
