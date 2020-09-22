/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "imagestego/core/intrinsic.hpp"
#ifdef _MSC_VER
#   include <intrin.h>
#   pragma intrinsic(_BitScanReverse)
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
