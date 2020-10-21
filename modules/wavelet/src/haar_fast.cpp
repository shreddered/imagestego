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
#include "imagestego/wavelet/haar_fast.hpp"
// c++ headers
#include <cstdint>
#include <cstring>
#include <future>
#include <vector>
// opencv
#include <opencv2/core.hpp>
 // x86 intrinsics
#include <x86intrin.h>


namespace imagestego {

class HaarFastWaveletImpl {
public:
    explicit HaarFastWaveletImpl() noexcept {}
    void setMatrix(const cv::Mat& src) {
        cv::split(src, _planes);
    }
    cv::Mat transform() {
        cv::Mat dst;
        std::vector<std::future<cv::Mat> > futures;
        std::vector<cv::Mat> planes;
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
private:
    // TODO: fix this
    static cv::Mat horizontalLifting(const cv::Mat& src) {
        cv::Mat dst(src.size(), CV_16SC1);
        const __m256i mask = _mm256_set_epi32(7, 6, 3, 2, 5, 4, 1, 0);
        for (int row = 0; row != src.rows; ++row) {
            const int16_t* sptr = src.ptr<int16_t>(row);
            int16_t* dptr = dst.ptr<int16_t>(row);
            const int aligned = align32(src.cols);
            for (int col = 0; col != aligned; col += 32) {
                const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(sptr + col)),
                              b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(sptr + col + 16)),
                              sum = _mm256_srai_epi16(_mm256_hadd_epi16(a, b), 1),
                              dif = _mm256_hsub_epi16(a, b);
                const __m256i lo = _mm256_permutevar8x32_epi32(sum, mask),
                              hi = _mm256_permutevar8x32_epi32(dif, mask);
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(dptr + col / 2), lo);
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(dptr + src.cols / 2 + col / 2), hi);
            }
            for (int col = aligned; col < src.cols - 1; col += 2) {
                *(dptr + col / 2) = floor2(sptr[col + 1] + sptr[col]);
                *(dptr + src.cols / 2 + col / 2) = sptr[col] - sptr[col + 1];
            }
            if (src.cols % 2 != 0) {
                dptr[src.cols - 1] = sptr[src.cols - 1];
            }
        }
        return dst;
    }
    static inline int16_t floor2(int16_t num) {
        return (num < 0) ? (num - 1) / 2 : num / 2;
    }
    static cv::Mat verticalLifting(const cv::Mat& src) {
        cv::Mat dst(src.size(), CV_16SC1);
        for (int row = 0; row < (src.rows & ~1); row += 2) {
            const int16_t* ptr1 = src.ptr<int16_t>(row);
            const int16_t* ptr2 = src.ptr<int16_t>(row + 1);
            int16_t* loptr = dst.ptr<int16_t>(row / 2);
            int16_t* hiptr = dst.ptr<int16_t>(row / 2 + src.rows / 2);
            const int aligned = align16(src.cols);
            for (int col = 0; col != aligned; col += 16) {
                const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr1 + col)),
                              b = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr2 + col));
                const __m256i lo = _mm256_srai_epi16(_mm256_add_epi16(a, b), 1),
                              hi = _mm256_sub_epi16(a, b);
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(loptr + col), lo);
                _mm256_storeu_si256(reinterpret_cast<__m256i*>(hiptr + col), hi);
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
        _mm256_zeroupper();
        return dst;
        // return horizontalLifting(src.t()).t();
    }
    static inline int align32(int num) {
        return num & ~0x1F;
    }
    static inline int align16(int num) {
        return num & ~0xF;
    }
    std::vector<cv::Mat> _planes;
}; // class HaarFastWaveletImpl

HaarFastWavelet::HaarFastWavelet() : pImpl(new HaarFastWaveletImpl) {}

HaarFastWavelet::~HaarFastWavelet() noexcept {
    if (pImpl)
        delete pImpl;
}

void HaarFastWavelet::setMatrix(const cv::Mat& src) {
    pImpl->setMatrix(src);
}

cv::Mat HaarFastWavelet::transform() {
    return pImpl->transform();
}

} // namespace imagestego
