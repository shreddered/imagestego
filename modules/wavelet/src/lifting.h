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
#ifndef __IMAGESTEGO_WAVELET_LIFTING_H_INCLUDED__
#define __IMAGESTEGO_WAVELET_LIFTING_H_INCLUDED__

// c headers
#include <stdint.h>

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

#ifdef __cplusplus
}
#endif

#endif /* __IMAGESTEGO_WAVELET_LIFTING_H_INCLUDED__ */
