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

void verticalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst,
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
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows - 1; row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align32(cols);
        size_t col;
        for (col = 0; col != aligned; col += 32) {
            // TODO: MASM implementation
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || (IMAGESTEGO_ICC && !IMAGESTEGO_WIN)
            asm(
                "vmovdqu16 (%[a], %[col], 2), %%zmm0 \n\t"
                "vmovdqu16 (%[b], %[col], 2), %%zmm1 \n\t"
                "vpaddw    %%zmm1, %%zmm0, %%zmm2    \n\t"
                "vpsraw    $0x1, %%zmm2, %%zmm2      \n\t"
                "vpsubw    %%zmm1, %%zmm0, %%zmm0    \n\t"
                "vmovdqu16 %%zmm2, (%[lo], %[col], 2)\n\t"
                "vmovdqu16 %%zmm0, (%[hi], %[col], 2)   \n\t"
                :
                : [lo]  "r" (loptr),
                  [hi]  "r" (hiptr),
                  [a]   "r" (ptr1),
                  [b]   "r" (ptr2),
                  [col] "r" (col)
                : "%zmm0", "%zmm1", "%zmm2", "memory"
            );
#endif
        }
        // TODO: MASM implementation
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || (IMAGESTEGO_ICC && !IMAGESTEGO_WIN)
            asm(
                "vmovdqu16 (%[a], %[col], 2), %%zmm0 %{%[mask]%}%{z%}\n\t"
                "vmovdqu16 (%[b], %[col], 2), %%zmm1 %{%[mask]%}%{z%}\n\t"
                "vpaddw    %%zmm1, %%zmm0, %%zmm2                    \n\t"
                "vpsraw    $0x1, %%zmm2, %%zmm2                      \n\t"
                "vpsubw    %%zmm1, %%zmm0, %%zmm0                    \n\t"
                "vmovdqu16 %%zmm2, (%[lo], %[col], 2) %{%[mask]%}    \n\t"
                "vmovdqu16 %%zmm0, (%[hi], %[col], 2) %{%[mask]%}    \n\t"
                :
                : [mask] "Yk" (len2mask[cols - col]),
                  [a]    "r" (ptr1),
                  [b]    "r" (ptr2),
                  [lo]   "r" (loptr),
                  [hi]   "r" (hiptr),
                  [col]  "r" (col)
                : "%zmm0", "%zmm1", "%zmm2", "memory"
            );
#endif
    }
    if (rows % 2 != 0) {
        memcpy(dst + (rows - 1) * cols,
               src + (rows - 1) * cols,
               cols * sizeof(int16_t));
    }
}

#endif /* IMAGESTEGO_AVX512BW_SUPPORTED */


#if IMAGESTEGO_AVX2_SUPPORTED && !IMAGESTEGO_AVX512BW_SUPPORTED

void verticalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst, const int rows,
        const int cols) {
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows - 1; row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align16(cols);
        for (size_t col = 0; col != aligned; col += 16) {
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || IMAGESTEGO_ICC
            asm(
                "vmovdqu (%[a], %[col], 2), %%ymm0 \n\t"
                "vmovdqu (%[b], %[col], 2), %%ymm1 \n\t"
                "vpaddw  %%ymm1, %%ymm0, %%ymm2    \n\t"
                "vpsraw  $0x1, %%ymm2, %%ymm2      \n\t"
                "vpsubw  %%ymm1, %%ymm0, %%ymm0    \n\t"
                "vmovdqu %%ymm2, (%[lo], %[col], 2)\n\t"
                "vmovdqu %%ymm0, (%[hi], %[col], 2)\n\t"
                :
                : [lo]  "r" (loptr),
                  [hi]  "r" (hiptr),
                  [a]   "r" (ptr1),
                  [b]   "r" (ptr2),
                  [col] "r" ((ptrdiff_t) col)
                : "%ymm0", "%ymm1", "%ymm2", "memory"
            );
#else
            const __m256i a = _mm256_loadu_si256((const __m256i*) (ptr1 + col)),
                          b = _mm256_loadu_si256((const __m256i*) (ptr2 + col));
            const __m256i lo = _mm256_srai_epi16(_mm256_add_epi16(a, b), 1),
                          hi = _mm256_sub_epi16(a, b);
            _mm256_storeu_si256((__m256i*) (loptr + col), lo);
            _mm256_storeu_si256((__m256i*) (hiptr + col), hi);
#endif
        }
        for (size_t col = aligned; col != cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
    }
    if (rows % 2 != 0) {
        memcpy(dst + (rows - 1) * cols, // NOLINT: allow memcpy usage
               src + (rows - 1) * cols,
               cols * sizeof(int16_t));
    }
}

#endif /* IMAGESTEGO_AVX2_SUPPORTED && !IMAGESTEGO_AVX512BW_SUPPORTED */

#if IMAGESTEGO_AVX2_SUPPORTED

void horizontalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst, const int rows,
        const int cols) {
    __m256i mask = _mm256_set_epi32(5, 4, 1, 0, 7, 6, 3, 2);
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows; ++row) {
        const int16_t* sptr = src + row * cols;
        int16_t* dptr = dst + row * cols;
        const int aligned = align32(cols);
        for (size_t col = 0; col != aligned; col += 32) {
            int16_t* tmp1 = dptr + col / 2, * tmp2 = tmp1 + cols / 2;
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || IMAGESTEGO_ICC
            asm(
                "vmovdqu (%[src], %[col], 2), %%ymm0  \n\t"
                "vmovdqu 32(%[src], %[col], 2), %%ymm1\n\t"
                "vphsubw %%ymm0, %%ymm1, %%ymm2       \n\t"
                "vphaddw %%ymm0, %%ymm1, %%ymm1       \n\t"
                "vpsraw  $0x1, %%ymm1, %%ymm1         \n\t"
                "vpermd  %%ymm1, %[mask], %%ymm1      \n\t"
                "vpermd  %%ymm2, %[mask], %%ymm2      \n\t"
                "vmovdqu %%ymm1, (%[lo])              \n\t"
                "vmovdqu %%ymm2, (%[hi])              \n\t"
                :
                : [lo]   "r" (tmp1),
                  [hi]   "r" (tmp2),
                  [src]  "r" (sptr),
                  [mask] "x" (mask),
                  [col]  "r" ((ptrdiff_t) col)
                : "%ymm0", "%ymm1", "%ymm2", "memory"
            );
#else // MSVC doesn't support inline asm for x64
            const __m256i a = _mm256_loadu_si256((const __m256i*) (src + col)),
                          b = _mm256_loadu_si256((const __m256i*) (src + col + 16));
            const __m256i lo = _mm256_srai_epi16(_mm256_hadd_epi16(b, a), 1),
                          hi = _mm256_hsub_epi16(b, a);
            _mm256_storeu_si256((__m256i*) tmp1, _mm256_permutevar8x32_epi32(lo, mask));
            _mm256_storeu_si256((__m256i*) tmp2, _mm256_permutevar8x32_epi32(hi, mask));
#endif
        }
        // TODO: implement with AVX512 if possible
        for (size_t col = aligned; col < cols - 1; col += 2) {
            *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
            *(dptr + cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
        }
        if (cols % 2 != 0) {
            dptr[cols - 1] = sptr[cols - 1];
        }
    }
}

#endif /* IMAGESTEGO_AVX2_SUPPORTED */

#if IMAGESTEGO_SSSE3_SUPPORTED && IMAGESTEGO_SSE2_SUPPORTED && !IMAGESTEGO_AVX2_SUPPORTED

void verticalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst, const int rows,
        const int cols) {
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows - 1; row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align8(cols);
        for (size_t col = 0; col != aligned; col += 8) {
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || (IMAGESTEGO_ICC && !IMAGESTEGO_WIN)
            asm(
                "movdqu (%[a], %[col], 2), %%xmm0 \n\t"
                "movdqu (%[b], %[col], 2), %%xmm1 \n\t"
                "movaps %%xmm0, %%xmm2            \n\t"
                "paddw  %%xmm1, %%xmm2            \n\t"
                "psraw  $0x1, %%xmm2              \n\t"
                "psubw  %%xmm1, %%xmm0            \n\t"
                "movdqu %%xmm2, (%[lo], %[col], 2)\n\t"
                "movdqu %%xmm0, (%[hi], %[col], 2)\n\t"
                :
                : [lo]  "r" (loptr),
                  [hi]  "r" (hiptr),
                  [a]   "r" (ptr1),
                  [b]   "r" (ptr2),
                  [col] "r" ((ptrdiff_t) col)
                : "%xmm0", "%xmm1", "%xmm2", "memory"
            );
            // TODO: add intrinsics implementation
#else
#endif
        }
        for (size_t col = aligned; col != cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
    }
    if (rows % 2 != 0) {
        memcpy(dst + (rows - 1) * cols, // NOLINT: allow memcpy usage
               src + (rows - 1) * cols,
               cols * sizeof(int16_t));
    }
}

void horizontalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst, const int rows,
        const int cols) {
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows; ++row) {
        const int16_t* sptr = src + row * cols;
        int16_t* dptr = dst + row * cols;
        const int aligned = align16(cols);
        for (size_t col = 0; col != aligned; col += 16) {
            int16_t* tmp1 = dptr + col / 2, * tmp2 = tmp1 + cols / 2;
            // TODO: add windows implementation
            asm(
                "movdqu (%[src], %[col], 2), %%xmm0  \n\t"
                "movdqu 16(%[src], %[col], 2), %%xmm1\n\t"
                "movaps %%xmm0, %%xmm2               \n\t"
                "phsubw %%xmm1, %%xmm2               \n\t"
                "phaddw %%xmm1, %%xmm0               \n\t"
                "psraw  $0x1, %%xmm0                 \n\t"
                "movdqu %%xmm0, (%[lo])              \n\t"
                "movdqu %%xmm2, (%[hi])              \n\t"
                :
                : [lo]  "r" (tmp1),
                  [hi]  "r" (tmp2),
                  [src] "r" (sptr),
                  [col] "r" ((ptrdiff_t) col)
                : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "memory"
            );
        }
        for (size_t col = aligned; col < cols - 1; col += 2) {
            *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
            *(dptr + cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
        }
        if (cols % 2 != 0) {
            dptr[cols - 1] = sptr[cols - 1];
        }
    }
}

#endif /* IMAGESTEGO_SSSE3_SUPPORTED && IMAGESTEGO_SSE2_SUPPORTED && !IMAGESTEGO_AVX2_SUPPORTED */

#if IMAGESTEGO_NEON_SUPPORTED

void verticalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* IMAGESTEGO_RESTRICT _dst, const int rows, const int cols) {
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < (rows & ~1); row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align8(cols);
        for (size_t col = 0; col != aligned; col += 8) {
            const int16x8_t a = vld1q_s16(ptr1 + col),
                            b = vld1q_s16(ptr2 + col);
            const int16x8_t lo = vhaddq_s16(a, b),
                            hi = vsubq_s16(a, b);
            vst1q_s16(loptr + col, lo);
            vst1q_s16(hiptr + col, hi);
        }
        for (size_t col = aligned; col != cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
    }
    if (rows % 2 != 0) {
        memcpy(dst + (rows - 1) * cols, // NOLINT: allow memcpy usage
               src + (rows - 1) * cols,
               cols * sizeof(int16_t));
    }
}

void horizontalHaar(const uint8_t* IMAGESTEGO_RESTRICT _src, uint8_t* _dst, const int rows, const int cols) {
    const int16_t* src = (const int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (size_t row = 0; row < rows; ++row) {
        const int16_t* sptr = src + row * cols;
        int16_t* dptr = dst + row * cols;
        const int aligned = align16(cols);
        for (size_t col = 0; col != aligned; col += 16) {
            const int16x8_t a = vld1q_s16(sptr + col),
                            b = vld1q_s16(sptr + col + 8);
            const int16x8_t lo = vshrq_n_s16(vpaddq_s16(a, b), 1);
            const int32x4_t tmp1 = vreinterpretq_s32_s16(a),
                            tmp2 = vreinterpretq_s32_s16(b);
            const int16x8_t t1 = vcombine_s16(vmovn_s32(tmp1), vmovn_s32(tmp2)),
                            t2 = vcombine_s16(vshrn_n_s32(tmp1, 16), vshrn_n_s32(tmp2, 16));
            const int16x8_t hi = vsubq_s16(t1, t2);
            vst1q_s16(dptr + col / 2, lo);
            vst1q_s16(dptr + col / 2 + cols / 2, hi);
        }
        for (size_t col = aligned; col < cols - 1; col += 2) {
            *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
            *(dptr + cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
        }
        if (cols % 2 != 0) {
            dptr[cols - 1] = sptr[cols - 1];
        }
    }
}

#endif /* IMAGESTEGO_NEON_SUPPORTED */

#ifdef __cplusplus
}
#endif
