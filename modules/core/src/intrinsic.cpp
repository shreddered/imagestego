/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// imagestego headers
#include "imagestego/core/intrinsic.hpp"
// c++ headers
#include <cstdlib>
#if IMAGESTEGO_MSVC && HAVE_INTRIN_H
#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
#endif

#if IMAGESTEGO_MSVC
#define bswap_32(x) _byteswap_ulong(x)
#elif IMAGESTEGO_ICC
#define bswap_32(x) _bswap(x)
#elif IMAGESTEGO_GCC || (IMAGESTEGO_CLANG && !defined(__APPLE__))
#define bswap_32(x) __builtin_bswap32(x)
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#elif defined(__sun) || defined(sun)
#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#elif defined(__OpenBSD__)
#include <sys/types.h>
#define bswap_32(x) swap32(x)
#elif defined(__NetBSD__)
#include <machine/bswap.h>
#include <sys/types.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#endif
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
