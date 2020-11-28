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

// imagestego headers
#include "imagestego/core/intrinsic.hpp"
// c++ headers
#include <cstdlib>
#if IMAGESTEGO_MSVC && HAVE_INTRIN_H
#   include <intrin.h>
#   pragma intrinsic(_BitScanReverse)
#endif

#if IMAGESTEGO_MSVC
#   define bswap_32(x) _byteswap_ulong(x)
#elif IMAGESTEGO_GCC || (IMAGESTEGO_CLANG && !defined(__APPLE__))
#   define bswap_32(x) __builtin_bswap32(x)
#elif defined(__APPLE__)
#   include <libkern/OSByteOrder.h>
#   define bswap_32(x) OSSwapInt32(x)
#elif defined(__sun) || defined(sun) 
#   include <sys/byteorder.h>
#   define bswap_32(x) BSWAP_32(x)
#elif defined(__FreeBSD__)
#   include <sys/endian.h>
#   define bswap_32(x) bswap32(x)
#elif defined(__OpenBSD__)
#   include <sys/types.h>
#   define bswap_32(x) swap32(x)
#elif defined(__NetBSD__)
#   include <sys/types.h>
#   include <machine/bswap.h>
#   if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#       define bswap_32(x) bswap32(x)
#   endif
#endif

namespace imagestego {

uint8_t log2(uint32_t value) noexcept {
#if IMAGESTEGO_CLANG || IMAGESTEGO_GCC
    return value ? 31 - __builtin_clz(value) : 0;
#elif IMAGESTEGO_ICC
    uint32_t result = 0;
    _BitScanReverse(&result, value);
    return result;
#elif IMAGESTEGO_MSVC
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
#ifdef bswap_32
    return bswap_32(value);
#else
    char* tmp = reinterpret_cast<char*>(&value);
    std::reverse(tmp, tmp + 4);
    return value;
#endif
}

} // namespace imagestego
