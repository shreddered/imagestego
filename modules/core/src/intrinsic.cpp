#include "imagestego/core/intrinsic.hpp"
#ifdef _MSC_VER
#   include <stdlib.h>
#elif !(defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__))
#   include <algorithm>
#endif


namespace imagestego {

uint8_t log2(uint32_t value) noexcept {
#if defined(__clang__) || defined(__GNUC__)
    return value ? 31 - __builtin_clz(value) : 0;
#elif defined(_MSC_VER) || defined(__INTEL_COMPILER)
    unsigned long result = 0;
    _BitScanReverse(&result, value);
    return result;
#else
    uint8_t res = 0;
    while (value >>= 1)
        ++res;
    return res;
#endif
}

uint32_t bswap(uint32_t value) noexcept {
#if defined(__clang__) || defined(__GNUC__)
    return __builtin_bswap32(value);
#elif defined(_MSC_VER)
    return _byteswap_ulong(value);
#elif defined(__INTEL_COMPILER)
    return _bswap(value);
#else
    char* tmp = &value;
    std::reverse(tmp, tmp + 4);
    return value;
#endif
}

} // namespace imagestego
