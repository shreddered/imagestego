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
#include "imagestego/wavelet/haar.hpp"
// c++ headers
#include <cstdint>
#include <future>
#include <vector>
// opencv headers
#include <opencv2/core.hpp>

// SIMD headers
#if defined(IMAGESTEGO_AVX2_SUPPORTED)
#   include <immintrin.h>
#elif defined(IMAGESTEGO_SSE2_SUPPORTED) || defined(IMAGESTEGO_SSSE3_SUPPORTED)
#   ifdef IMAGESTEGO_SSE2_SUPPORTED
#       include <emmintrin.h>
#   endif
#   ifdef IMAGESTEGO_SSSE3_SUPPORTED
#       include <tmmintrin.h>
#   endif
#elif defined(IMAGESTEGO_NEON_SUPPORTED)
#   include <arm_neon.h>
#elif defined(IMAGESTEGO_ALTIVEC_SUPPORTED)
#   include <altivec.h>
#endif

extern "C" {

void vertical_lifting(const uint8_t*, uint8_t*, const int, const int);

void horizontal_lifting(const uint8_t*, uint8_t*, const int, const int);

}

namespace imagestego {

class HaarWaveletImpl final {
public:
    explicit HaarWaveletImpl() noexcept {}
    cv::Mat transform(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<std::future<cv::Mat> > futures;
        std::vector<cv::Mat> planes, _planes;
        cv::split(mat, _planes);
        for (cv::Mat mat : _planes) {
            futures.emplace_back(std::async([](cv::Mat src) {
                src.convertTo(src, CV_16S);
                return verticalLifting(horizontalLifting(src));
            }, mat));
        }
        for (auto&& f : futures) {
            planes.push_back(f.get());
        }
        cv::merge(planes, dst);
        return dst;
    }
    cv::Mat inverse(const cv::Mat& mat) {
        // TODO: implement inverse wavelet
        return mat;
    }
private:
    static cv::Mat horizontalLifting(const cv::Mat& src) {
        cv::Mat dst(src.size(), CV_16S);
        src.copyTo(dst);
        auto x = src.cols >> 1;
        for (int i = 0; i != src.rows; ++i) {
            for (int j = 0; j != x; ++j) {
                auto a = src.at<int16_t>(i, (j << 1)),
                     b = src.at<int16_t>(i, (j << 1) + 1);
                dst.at<int16_t>(i, j) = floor2(a + b);
                dst.at<int16_t>(i, j + x) = a - b;
            }
        }
        return dst;
    }
    static inline cv::Mat verticalLifting(const cv::Mat& src) {
        return horizontalLifting(src.t()).t();
    }
    static inline int floor2(int num) {
        return (num < 0) ? (num - 1) / 2 : num / 2;
    }
}; // class HaarWaveletImpl

HaarWavelet::HaarWavelet() : pImpl(new HaarWaveletImpl) {}

HaarWavelet::~HaarWavelet() noexcept {
    if (pImpl)
        delete pImpl;
}

cv::Mat HaarWavelet::transform(const cv::Mat& mat) {
    return pImpl->transform(mat);
}

cv::Mat HaarWavelet::inverse(const cv::Mat& mat) {
    return pImpl->inverse(mat);
}

namespace experimental {

class HaarWaveletImpl {
public:
    explicit HaarWaveletImpl() noexcept {}
    cv::Mat transform(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<std::future<cv::Mat> > futures;
        std::vector<cv::Mat> planes, _planes;
        cv::split(mat, _planes);
        for (const cv::Mat& mat : _planes) {
            futures.emplace_back(std::async([](cv::Mat mat) -> cv::Mat {
                mat.convertTo(mat, CV_16S);
                return verticalLifting(horizontalLifting(mat));
            }, std::cref(mat)));
        }
        for (auto&& f : futures) {
            planes.emplace_back(f.get());
        }
        cv::merge(planes, dst);
        return dst;
    }
    cv::Mat inverse(const cv::Mat& mat) {
        // TODO: implement inverse wavelet
        return mat;
    }
private:
    static cv::Mat horizontalLifting(const cv::Mat& src);
    static cv::Mat verticalLifting(const cv::Mat& src);
    static inline int16_t floor2(int16_t num) {
        return (num < 0) ? (num - 1) / 2 : num / 2;
    }
    static inline int align32(int num) {
        return num & ~0x1F;
    }
    static inline int align16(int num) {
        return num & ~0xF;
    }
#if defined(IMAGESTEGO_NEON_SUPPORTED) || defined(IMAGESTEGO_SSE2_SUPPORTED)
    static inline int align8(int num) {
        return num & ~0x7;
    }
#endif
}; // class HaarWaveletImpl

#if defined(IMAGESTEGO_AVX2_SUPPORTED)
cv::Mat HaarWaveletImpl::horizontalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    horizontal_lifting(src.data, dst.data, src.rows, src.cols);
    return dst;
}
#elif defined(IMAGESTEGO_SSSE3_SUPPORTED) && defined(IMAGESTEGO_SSE2_SUPPORTED)
cv::Mat HaarWaveletImpl::horizontalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    horizontal_lifting(src.data, dst.data, src.rows, src.cols);
    return dst;
}
// TODO: vectorize loop using ARM NEON intrinsics
#elif defined(IMAGESTEGO_NEON_SUPPORTED)
cv::Mat HaarWaveletImpl::horizontalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    // for (int row = 0; row != src.rows; ++row) {
        // const int16_t* sptr = src.ptr<int16_t>(row);
        // int16_t* dptr = dst.ptr<int16_t>(row);
        // const int aligned = align16(src.cols);
        // for (int col = 0; col != aligned; col += 16) {
            // const int16x8_t a = vld1q_s16(sptr + col),
                            // b = vld1q_s16(sptr + col + 8),
                            // sum = vpaddq_s16(a, b);
        // }
    // }
    dst = verticalLifting(src.t()).t();
    return dst;
}
#endif

#if defined(IMAGESTEGO_AVX2_SUPPORTED)
cv::Mat HaarWaveletImpl::verticalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    vertical_lifting(src.data, dst.data, src.rows, src.cols);
    return dst;
}
#elif defined(IMAGESTEGO_SSE2_SUPPORTED)
cv::Mat HaarWaveletImpl::verticalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    // for (int row = 0; row < (src.rows & ~1); row += 2) {
        // const int16_t* ptr1 = src.ptr<int16_t>(row);
        // const int16_t* ptr2 = src.ptr<int16_t>(row + 1);
        // int16_t* loptr = dst.ptr<int16_t>(row / 2);
        // int16_t* hiptr = dst.ptr<int16_t>(row / 2 + src.rows / 2);
        // const int aligned = align8(src.cols);
        // for (int col = 0; col != aligned; col += 8) {
            // const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr1 + col)),
                          // b = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr2 + col));
            // const __m128i lo = _mm_srai_epi16(_mm_add_epi16(a, b), 1),
                          // hi = _mm_sub_epi16(a, b);
            // _mm_storeu_si128(reinterpret_cast<__m128i*>(loptr + col), lo);
            // _mm_storeu_si128(reinterpret_cast<__m128i*>(hiptr + col), hi);
        // }
        // for (int col = aligned; col != src.cols; ++col) {
            // loptr[col] = floor2(ptr1[col] + ptr2[col]);
            // hiptr[col] = ptr1[col] - ptr2[col];
        // }
    // }
    // if (src.rows % 2 != 0) {
        // memcpy(reinterpret_cast<void*>(dst.ptr<int16_t>(src.rows - 1)),
               // reinterpret_cast<const void*>(src.ptr<int16_t>(src.rows - 1)),
               // src.cols * sizeof(int16_t));
    // }
    vertical_lifting(src.data, dst.data, src.rows, src.cols);
    return dst;
}
#elif defined(IMAGESTEGO_NEON_SUPPORTED)
cv::Mat HaarWaveletImpl::verticalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    for (int row = 0; row < (src.rows & ~1); row += 2) {
        const int16_t* ptr1 = src.ptr<int16_t>(row);
        const int16_t* ptr2 = src.ptr<int16_t>(row + 1);
        int16_t* loptr = dst.ptr<int16_t>(row / 2);
        int16_t* hiptr = dst.ptr<int16_t>(row / 2 + src.rows / 2);
        const int aligned = align8(src.cols);
        for (int col = 0; col != aligned; col += 8) {
            const int16x8_t a = vld1q_s16(ptr1 + col),
                            b = vld1q_s16(ptr2 + col);
            const int16x8_t lo = vhaddq_s16(a, b),
                            hi = vsubq_s16(a, b);
            vst1q_s16(loptr + col, lo);
            vst1q_s16(hiptr + col, hi);
        }
        for (int col = aligned; col != src.cols; ++col) {
            loptr[col] = floor2(ptr1[col] + ptr2[col]);
            hiptr[col] = ptr1[col] - ptr2[col];
        }
    }
    if (src.rows % 2 != 0) {
        memcpy(reinterpret_cast<void*>(dst.ptr<int16_t>(src.rows - 1)),
               reinterpret_cast<const void*>(src.ptr<int16_t>(src.rows - 1)),
               src.cols * sizeof(int16_t));
    }
    return dst;
}
#endif

HaarWavelet::HaarWavelet() : pImpl(new HaarWaveletImpl) {}

HaarWavelet::~HaarWavelet() noexcept {
    if (pImpl) {
        delete pImpl;
    }
}

cv::Mat HaarWavelet::transform(const cv::Mat& mat) {
    return pImpl->transform(mat);
}

cv::Mat HaarWavelet::inverse(const cv::Mat& mat) {
    return pImpl->inverse(mat);
}

} // namespace experimental

} // namespace imagestego
