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

#ifndef __IMAGESTEGO_WAVELET_HAAR_HPP_INCLUDED__
#define __IMAGESTEGO_WAVELET_HAAR_HPP_INCLUDED__

#include "imagestego/core/config.hpp"
#include "imagestego/wavelet/interfaces.hpp"


namespace imagestego {

class HaarWaveletImpl;

class IMAGESTEGO_EXPORTS HaarWavelet : public AbstractWavelet {
public:
    explicit HaarWavelet();
    void setMatrix(const cv::Mat& src) override;
    cv::Mat transform() override;
    virtual ~HaarWavelet() noexcept;
private:
    HaarWaveletImpl* pImpl;
};

namespace experimental {

class HaarWaveletImpl;

class IMAGESTEGO_EXPORTS HaarWavelet : public AbstractWavelet {
public:
    explicit HaarWavelet();
    void setMatrix(const cv::Mat& src) override;
    cv::Mat transform() override;
    virtual ~HaarWavelet() noexcept;
private:
    HaarWaveletImpl* pImpl;
};

} // namespace experimental

} // namespace imagestego

#endif /* __IMAGESTEGO_WAVELET_HAAR_HPP_INCLUDED__ */
