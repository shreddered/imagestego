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


namespace imagestego {

class HaarWaveletImpl final {
public:
    explicit HaarWaveletImpl() noexcept {}
    void setMatrix(const cv::Mat& src) {
        cv::split(src, _planes);
    }
    cv::Mat transform() {
        cv::Mat dst;
        std::vector<std::future<cv::Mat> > futures;
        std::vector<cv::Mat> planes;
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
    std::vector<cv::Mat> _planes;
}; // class HaarWaveletImpl

HaarWavelet::HaarWavelet() : pImpl(new HaarWaveletImpl) {}

HaarWavelet::~HaarWavelet() noexcept {
    if (pImpl)
        delete pImpl;
}

void HaarWavelet::setMatrix(const cv::Mat& mat) {
    pImpl->setMatrix(mat);
}

cv::Mat HaarWavelet::transform() {
    return pImpl->transform();
}

} // namespace imagestego
