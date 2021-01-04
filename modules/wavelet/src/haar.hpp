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
#ifndef __IMAGESTEGO_WAVELET_PRIVATE_HAAR_HPP_INCLUDED__
#define __IMAGESTEGO_WAVELET_PRIVATE_HAAR_HPP_INCLUDED__

// c headers
#include <cstdint>

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
void vertical_haar(const uint8_t* src, uint8_t* dst, const int rows, const int cols);

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
void horizontal_haar(const uint8_t* src, uint8_t* dst, int rows, int cols);

#ifdef __cplusplus
}
#endif

#endif /* __IMAGESTEGO_WAVELET_PRIVATE_HAAR_HPP_INCLUDED__ */
