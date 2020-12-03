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

// c headers
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// SIMD headers
#if IMAGESTEGO_AVX2_SUPPORTED || IMAGESTEGO_AVX512VL_SUPPORTED || IMAGESTEGO_AVX512BW_SUPPORTED
#   include <immintrin.h>
#elif IMAGESTEGO_SSE2_SUPPORTED || IMAGESTEGO_SSSE3_SUPPORTED
#   ifdef IMAGESTEGO_SSE2_SUPPORTED
#       include <emmintrin.h>
#   endif
#   ifdef IMAGESTEGO_SSSE3_SUPPORTED
#       include <tmmintrin.h>
#   endif
#elif IMAGESTEGO_NEON_SUPPORTED
#   include <arm_neon.h>
#elif IMAGESTEGO_ALTIVEC_SUPPORTED
#   include <altivec.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

// private function
static IMAGESTEGO_INLINE int16_t floor2(int16_t num) {
    return (num < 0) ? (num - 1) / 2 : num / 2;
}

// set of align functions
#if IMAGESTEGO_AVX512BW_SUPPORTED || IMAGESTEGO_AVX2_SUPPORTED

static IMAGESTEGO_INLINE int align32(const int num) {
    return num & ~0x1f;
}

#endif

#if IMAGESTEGO_AVX2_SUPPORTED || IMAGESTEGO_SSSE3_SUPPORTED || IMAGESTEGO_NEON_SUPPORTED

static IMAGESTEGO_INLINE int align16(const int num) {
    return num & ~0xf;
}

#endif

#if IMAGESTEGO_NEON_SUPPORTED || IMAGESTEGO_SSE2_SUPPORTED

static IMAGESTEGO_INLINE int align8(const int num) {
    return num & ~0x7;
}

#endif

// Extension-specific implementation goes here
#if IMAGESTEGO_AVX512BW_SUPPORTED

void inverse_vertical_haar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst,
        const int rows, const int cols) {
    static const __mmask32 len2mask[] = { 0x00000000, 0x00000001, 0x00000003, 0x00000007,
                                          0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
                                          0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
                                          0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
                                          0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
                                          0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
                                          0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
                                          0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
                                          0xffffffff };
    const __m512i zero = _mm512_setzero();
    int16_t* src = (int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (int row = 0; row != (rows & ~1); ++row) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align32(cols);
        int col;
        for (col = 0; col != aligned; col += 32) {
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || IMAGESTEGO_ICC
            asm(
                "vmovdqu16 (%[lo], %[col], 2), %%zmm0\n\t"
                "vmovdqu16 (%[hi], %[col], 2), %%zmm1\n\t"
                "vpsraw    $0x1, %%zmm1, %%zmm2      \n\t" // zmm2 = floor(hi / 2)
                "vpsubw    %%zmm2, %%zmm0, %%zmm2    \n\t" // zmm2 = lo - floor(hi / w)
                "vpavgw    %%zmm1, %[zero], %%zmm1   \n\t" // floor((zmm1 + 1) / 2)
                "vpaddw    %%zmm1, %%zmm0, %%zmm1    \n\t" // zmm1 = lo + floor((hi + 1 / 2) 
                "vmovdqu16 %%zmm2, (%[b], %[col], 2) \n\t"
                "vmovdqu16 %%zmm1, (%[a], %[col], 2) \n\t"
                :
                : [lo]   "r" (loptr),
                  [hi]   "r" (hiptr),
                  [a]    "r" (ptr1),
                  [zero] "x" (zero),
                  [b]    "r" (ptr2),
                  [col]  "r" ((ptrdiff_t) col)
                : "%zmm0", "%zmm1", "%zmm2", "memory"
            );
#else
            const __m512i lo = _mm512_loadu_epi16(loptr + col),
                          hi = _mm512_loadu_epi16(hiptr + col);
            const __m512i a = _mm512_add_epi16(lo, _mm512_avg_epu16(hi, zero)),
                          b = _mm512_sub_epi16(lo, _mm512_srai_epi16(hi, 1));
            _mm512_storeu_si512(ptr1 + col, a);
            _mm512_storeu_si512(ptr2 + col, b);
#endif
        }
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || IMAGESTEGO_ICC
        asm(
                "vmovdqu16 (%[lo], %[col], 2), %%zmm0 %{%[mask]%}%{z%}\n\t"
                "vmovdqu16 (%[hi], %[col], 2), %%zmm1 %{%[mask]%}%{z%}\n\t"
                "vpsraw    $0x1, %%zmm1, %%zmm2      \n\t"
                "vpsubw    %%zmm2, %%zmm0, %%zmm2    \n\t"
                "vpavgw    %%zmm1, %[zero], %%zmm1   \n\t"
                "vpaddw    %%zmm1, %%zmm0, %%zmm1    \n\t"
                "vmovdqu16 %%zmm2, (%[b], %[col], 2) %{%[mask]%}%{z%} \n\t"
                "vmovdqu16 %%zmm1, (%[a], %[col], 2) %{%[mask]%}%{z%} \n\t"
                :
                : [lo]   "r" (loptr),
                  [hi]   "r" (hiptr),
                  [a]    "r" (ptr1),
                  [zero] "x" (zero),
                  [b]    "r" (ptr2),
                  [mask] "Yk" (len2mask[cols - col]),
                  [col]  "r" ((ptrdiff_t) col)
                : "%zmm0", "%zmm1", "%zmm2", "memory"
            );
#else
        const __mmask32 msk = len2mask[cols - col]; 
        const __m512i lo = _mm512_maskz_loadu_epi16(msk, loptr + col),
                      hi = _mm512_maskz_loadu_epi16(msk, hiptr + col);
        const __m512i a = _mm512_add_epi16(lo, _mm512_avg_epu16(hi, zero)),
                      b = _mm512_sub_epi16(lo, _mm512_srai_epi16(hi, 1));
        _mm512_mask_storeu_epi16(ptr1 + col, msk, a);
        _mm512_mask_storeu_epi16(ptr2 + col, msk, b);
#endif
    }
    if (rows % 2 != 0) {
        memcpy(dst + (rows - 1) * cols,
               src + (rows - 1) * cols,
               cols * sizeof(int16_t));
    }
}

#endif

#ifdef __cplusplus
}
#endif
