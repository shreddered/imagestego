#include "core.hpp"


imagestego::Exception::Exception(const int& _code) noexcept : std::exception(), code(_code) {}

const char* imagestego::Exception::what() const noexcept {
    if (code & 1)
        return "No such file";
    if (code & (1 << 1))
        return "Cannot embed message: image is too small";
    if (code & (1 << 2))
        return "No key found"; 
    if (code & (1 << 3))
        return "Internal error";
    if (code & (1 << 4))
        return "Unknown LSB mode";
}
