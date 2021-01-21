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
#include "imagestego/wavelet/haar.hpp"
#include "haar.hpp"
#include "inverse_haar.hpp"
// c++ headers
#include <cstdint>
#include <future>
#include <vector>
// opencv headers
#include <opencv2/core.hpp>

namespace imagestego {

namespace impl {

class HaarWavelet final {
public:
    explicit HaarWavelet() noexcept {}
    cv::Mat transform(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<cv::Mat> planes, _planes;
        if (mat.depth() != CV_16S) {
            mat.convertTo(dst, CV_16S);
            cv::split(dst, _planes);
        } else {
            cv::split(mat, _planes);
        }
        std::vector<std::future<cv::Mat>> futures;
        futures.reserve(_planes.size() - 1);
        for (std::size_t i = 1; i != _planes.size(); ++i) {
            futures.emplace_back(std::async(
                [](const cv::Mat& src) {
                    return verticalLifting(horizontalLifting(src));
                },
                std::cref(_planes[i])));
        }
        planes.reserve(_planes.size());
        planes.emplace_back(verticalLifting(horizontalLifting(_planes.front())));
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
        std::vector<std::future<cv::Mat>> futures;
        futures.reserve(_planes.size());
        for (const cv::Mat& mat : _planes) {
            futures.emplace_back(std::async(
                [](const cv::Mat& src) {
                    return inverseHorizontalLifting(inverseVerticalLifting(src));
                },
                std::cref(mat)));
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
                auto a = src.at<short>(i, j), b = src.at<short>(i, j + x);
                dst.at<short>(i, (j << 1)) = a + floor2(b + 1);
                dst.at<short>(i, (j << 1) + 1) = a - floor2(b);
            }
        }
        return src;
    }
    static inline cv::Mat inverseVerticalLifting(const cv::Mat& src) {
        return inverseHorizontalLifting(src.t());
    }
    static inline int floor2(int num) { return (num < 0) ? (num - 1) / 2 : num / 2; }
}; // class HaarWavelet

} // namespace impl

HaarWavelet::HaarWavelet() : pImpl(new impl::HaarWavelet) {}

HaarWavelet::~HaarWavelet() noexcept {
    if (pImpl)
        delete pImpl;
}

cv::Mat HaarWavelet::transform(const cv::Mat& mat) { return pImpl->transform(mat); }

cv::Mat HaarWavelet::inverse(const cv::Mat& mat) { return pImpl->inverse(mat); }

namespace experimental {

namespace impl {

class HaarWavelet {
public:
    explicit HaarWavelet() noexcept {}
    cv::Mat transform(const cv::Mat& mat) {
        cv::Mat dst;
        std::vector<std::future<cv::Mat>> futures;
        std::vector<cv::Mat> planes, _planes;
        if (mat.depth() != CV_16S) {
            mat.convertTo(dst, CV_16S);
            cv::split(dst, _planes);
        } else {
            cv::split(mat, _planes);
        }

        for (std::size_t i = 1; i != _planes.size(); ++i) {
            futures.emplace_back(std::async(
                [](const cv::Mat& mat) -> cv::Mat {
                    return verticalLifting(horizontalLifting(mat));
                },
                std::cref(_planes[i])));
        }
        planes.reserve(_planes.size());
        planes.emplace_back(verticalLifting(horizontalLifting(_planes.front())));
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
}; // class HaarWavelet

cv::Mat HaarWavelet::horizontalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    horizontal_haar(src.data, dst.data, src.rows, src.cols);
    return dst;
}

cv::Mat HaarWavelet::verticalLifting(const cv::Mat& src) {
    cv::Mat dst(src.size(), CV_16SC1);
    vertical_haar(src.data, dst.data, src.rows, src.cols);
    return dst;
}

} // namespace impl

HaarWavelet::HaarWavelet() : pImpl(new impl::HaarWavelet) {}

HaarWavelet::~HaarWavelet() noexcept {
    if (pImpl) {
        delete pImpl;
    }
}

cv::Mat HaarWavelet::transform(const cv::Mat& mat) { return pImpl->transform(mat); }

cv::Mat HaarWavelet::inverse(const cv::Mat& mat) { return pImpl->inverse(mat); }

} // namespace experimental

} // namespace imagestego
