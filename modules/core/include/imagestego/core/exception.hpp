#ifndef __IMAGESTEGO_EXCEPTION_HPP_INCLUDED__
#define __IMAGESTEGO_EXCEPTION_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
// c++ headers
#include <stdexcept>


namespace imagestego {

class IMAGESTEGO_EXPORTS Exception : public std::exception {
public:
    explicit Exception(const int& _code) noexcept;
    const char* what() const noexcept override;
    enum Codes {
        NoSuchFile = 1,
        BigMessageSize = 1 << 1,
        NoKeyFound = 1 << 2,
        InternalError = 1 << 3,
        UnknownLsbMode = 1 << 4,
        NotJpegClass = 1 << 5
    };
private:
    int code;
};

} // namespace imagestego

#ifdef _MSC_VER
inline const char* imagestego::Exception::what() const noexcept {
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

#endif /* __IMAGESTEGO_EXCEPTION_HPP_INCLUDED__ */
