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
#include "haar.h"
// c++ headers
#include <cstdint>
#include <future>
#include <vector>
// opencv headers
#include <opencv2/core.hpp>


namespace imagestego {

class HaarWaveletImpl final {
public:
    explicit HaarWaveletImpl() noexcept {}
    cv::Mat transform(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<cv::Mat> planes, _planes;
        if (mat.depth() != CV_16S) {
            mat.convertTo(dst, CV_16S);
            cv::split(dst, _planes);
        }
        else {
            cv::split(mat, _planes);
        }
        std::vector<std::future<cv::Mat> > futures;
        futures.reserve(_planes.size());
        for (const cv::Mat& mat : _planes) {
            futures.emplace_back(std::async([](const cv::Mat& src) {
                return verticalLifting(horizontalLifting(src));
            }, std::cref(mat)));
        }
        planes.reserve(_planes.size());
        for (auto&& f : futures) {
            planes.emplace_back(f.get());
        }
        cv::merge(planes, dst);
        return dst;
    }
    cv::Mat inverse(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<cv::Mat> planes, _planes;
        cv::split(mat, _planes);
        std::vector<std::future<cv::Mat> > futures;
        futures.reserve(_planes.size());
        for (const cv::Mat& mat : _planes) {
            futures.emplace_back(std::async([](const cv::Mat& src) {
                return inverseHorizontalLifting(inverseVerticalLifting(src));
            }, std::cref(mat)));
        }
        planes.reserve(_planes.size());
        for (auto&& f : futures) {
            planes.emplace_back(f.get());
        }
        cv::merge(planes, dst);
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
    static cv::Mat inverseHorizontalLifting(const cv::Mat& src) {
        cv::Mat dst(src.size(), CV_16SC1);
        src.copyTo(dst);
        auto x = src.cols >> 1;
        for (int i = 0; i != src.rows; ++i) {
            for (int j = 0; j != x; ++j) {
                auto a = src.at<short>(i, j),
                     b = src.at<short>(i, j + x);
                dst.at<short>(i, (j << 1)) = a + floor2(b + 1);
                dst.at<short>(i, (j << 1) + 1) = a - floor2(b);
            }
        }
        return src;
    }
    static inline cv::Mat inverseVerticalLifting(const cv::Mat& src) {
        return inverseHorizontalLifting(src.t());
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
        if (mat.depth() != CV_16S) {
            mat.convertTo(dst, CV_16S);
            cv::split(dst, _planes);
        }
        else {
            cv::split(mat, _planes);
        }
        for (const cv::Mat& mat : _planes) {
            futures.emplace_back(std::async([](const cv::Mat& mat) -> cv::Mat {
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
}; // class HaarWaveletImpl

cv::Mat HaarWaveletImpl::horizontalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    horizontal_haar(src.data, dst.data, src.rows, src.cols);
    return dst;
}

cv::Mat HaarWaveletImpl::verticalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    vertical_haar(src.data, dst.data, src.rows, src.cols);
    return dst;
}

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
