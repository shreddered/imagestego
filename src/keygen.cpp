#include "imagestego/keygen.hpp"


namespace imagestego {

std::string keygen::generate(const uint32_t& len) {
    std::random_device rd;
    std::uniform_int_distribution<char> dist(32, 126);
    std::string s;
    s.reserve(len);
    for (uint32_t i = 0; i != len; ++i) {
        s[i] = dist(rd);
    }
    return s;
}

} // namespace imagestego
