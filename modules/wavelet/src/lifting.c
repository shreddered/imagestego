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
#include <stdint.h>
#include <string.h>

// SIMD headers
#if IMAGESTEGO_AVX2_SUPPORTED || (IMAGESTEGO_AVX512VL_SUPPORTED && IMAGESTEGO_AVX512BW_SUPPORTED)
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

/**
 * Function which computes vertical lifting.
 *
 * NB: pointers only from CV_16S matrices are accepted.
 *
 * @param src Pointer to source matrix obtained from cv::Mat::data.
 * @param dst Destination pointer.
 * @param rows Number of rows.
 * @param cols Number of columns.
 */
void vertical_lifting(const uint8_t* src, uint8_t* dst, const int rows, const int cols);

/**
 * Function which computes horizontal lifting.
 *
 * NB: pointers only from CV_16S matrices are accepted.
 *
 * @param src Pointer to source matrix obtained from cv::Mat::data.
 * @param dst Destination pointer.
 * @param rows Number of rows.
 * @param cols Number of columns.
 */
void horizontal_lifting(const uint8_t* src, uint8_t* dst, int rows, int cols);

// private function
static int16_t floor2(int16_t num) {
    return (num < 0) ? (num - 1) / 2 : num / 2;
}

// Extension-specific implementation goes here
#if IMAGESTEGO_AVX2_SUPPORTED

int align32(const int num) {
    return num & ~0x1f;
}

int align16(const int num) {
    return num & ~0xf;
}

void vertical_lifting(const uint8_t* restrict _src, uint8_t* restrict _dst, const int rows, const int cols) {
    int16_t* src = (int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (int row = 0; row != (rows & ~1); row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align16(cols);
        for (int col = 0; col != aligned; col += 16) {
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || (IMAGESTEGO_ICC && !IMAGESTEGO_WIN)
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
                  [col] "r" ((ssize_t) col)
                : "%ymm0", "%ymm1", "%ymm2", "memory"
            );
#elif IMAGESTEGO_WIN
            __asm {
                vmovdqu ymm0, [ptr1 + col * 2]
                vmovdqu ymm1, [ptr2 + col * 2]
                vpaddw  ymm2, ymm0, ymm1
                vpsraw  ymm2, ymm2, 1
                vpsubw  ymm0, ymm0, ymm1
                vmovdqu [loptr + col * 2], ymm2
                vmovdqu [hiptr + col * 2], ymm0
            };
#endif
        }
        // TODO: implement with AVX512 if possible
        for (int col = aligned; col != cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
        if (rows % 2 != 0) {
            memcpy(dst + (rows - 1) * cols,
                   src + (rows - 1) * cols,
                   cols * sizeof(int16_t));
        }
    }
}

void horizontal_lifting(const uint8_t* restrict _src, uint8_t* restrict _dst, const int rows, const int cols) {
    static const uint32_t mask[] = {2, 3, 6, 7, 0, 1, 4, 5};
    int16_t* src = (int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (int row = 0; row != rows; ++row) {
        const int16_t* sptr = src + row * cols;
        int16_t* dptr = dst + row * cols;
        const int aligned = align32(cols);
        for (int col = 0; col != aligned; col += 32) {
            int16_t* tmp1 = dptr + col / 2, * tmp2 = tmp1 + cols / 2;
#if IMAGESTEGO_GCC || IMAGESTEGO_CLANG || (IMAGESTEGO_ICC && !IMAGESTEGO_WIN)
            asm(
                "vmovdqu (%[src], %[col], 2), %%ymm0  \n\t"
                "vmovdqu 32(%[src], %[col], 2), %%ymm1\n\t"
                "vmovdqu (%[mask]), %%ymm3            \n\t"
                "vphsubw %%ymm0, %%ymm1, %%ymm2       \n\t"
                "vphaddw %%ymm0, %%ymm1, %%ymm1       \n\t"
                "vpsraw  $0x1, %%ymm1, %%ymm1         \n\t"
                "vpermd  %%ymm1, %%ymm3, %%ymm1       \n\t"
                "vpermd  %%ymm2, %%ymm3, %%ymm2       \n\t"
                "vmovdqu %%ymm1, (%[lo])              \n\t"
                "vmovdqu %%ymm2, (%[hi])              \n\t"
                :
                : [lo]   "r" (tmp1),
                  [hi]   "r" (tmp2),
                  [src]  "r" (sptr),
                  [mask] "r" ((uint32_t*) mask),
                  [col]  "r" ((ssize_t) col)
                : "%ymm0", "%ymm1", "%ymm2", "%ymm3", "memory"
            );
#elif IMAGESTEGO_WIN
            __asm {
                vmovdqu ymm0, [ptr1 + col * 2]
                vmovdqu ymm1, [ptr2 + col * 2 + 32]
                vmovdqu ymm3, [mask]
                vphsubw ymm2, ymm1, ymm0
                vphaddw ymm1, ymm1, ymm0
                vpsraw  ymm1, ymm1, 1
                vpermd  ymm1, ymm3, ymm1
                vpermd  ymm2, ymm3, ymm2
                vmovdqu [tmp1], ymm1
                vmovdqu [tmp2], ymm2
            };
#endif
        }
        // TODO: implement with AVX512 if possible
        for (int col = aligned; col < cols - 1; col += 2) {
            *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
            *(dptr + cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
        }
        if (cols % 2 != 0) {
            dptr[cols - 1] = sptr[cols - 1];
        }
    }
}

#elif IMAGESTEGO_SSSE3_SUPPORTED && IMAGESTEGO_SSE2_SUPPORTED

int align16(const int num) {
    return num & ~0xf;
}

void vertical_lifting(const uint8_t* restrict _src, uint8_t* restrict _dst, const int rows, const int cols) {
    int16_t* src = (int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (int row = 0; row != (rows & ~1); row += 2) {
        const int16_t* ptr1 = src + row * cols;
        const int16_t* ptr2 = src + (row + 1) * cols;
        int16_t* loptr = dst + (row / 2) * cols;
        int16_t* hiptr = dst + (row / 2 + rows / 2) * cols;
        const int aligned = align16(cols);
        for (int col = 0; col != aligned; col += 8) {
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
                  [col] "r" ((ssize_t) col)
                : "%xmm0", "%xmm1", "%xmm2", "memory"
                );
            // TODO: add windows implementation
#elif IMAGESTEGO_WIN
            __asm {
            };
#endif
        }
        for (int col = aligned; col != cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
        if (rows % 2 != 0) {
            memcpy(dst + (rows - 1) * cols,
                   src + (rows - 1) * cols,
                   cols * sizeof(int16_t));
        }
    }
}

void horizontal_lifting(const uint8_t* restrict _src, uint8_t* restrict _dst, const int rows, const int cols) {
    int16_t* src = (int16_t*) _src;
    int16_t* dst = (int16_t*) _dst;
    for (int row = 0; row != rows; ++row) {
        const int16_t* sptr = src + row * cols;
        int16_t* dptr = dst + row * cols;
        const int aligned = align16(cols);
        for (int col = 0; col != aligned; col += 16) {
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
                  [col] "r" ((ssize_t) col)
                : "%xmm0", "%xmm1", "%xmm2", "%xmm3", "memory"
            );
        }
        for (int col = aligned; col < cols - 1; col += 2) {
            *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
            *(dptr + cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
        }
        if (cols % 2 != 0) {
            dptr[cols - 1] = sptr[cols - 1];
        }
    }
}

#endif

#ifdef __cplusplus
}
#endif
