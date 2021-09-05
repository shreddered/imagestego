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

#ifndef __IMAGESTEGO_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_LSB_HPP_INCLUDED__

// imagestego headers
#included "imagestego/core/export.hpp"
#include "imagestego/core/types.hpp"
// c++ headers
#include <string>
// opencv headers
#include <opencv2/core/mat.hpp>

namespace imagestego {

namespace lsb {

/**
 * @brief A function that performs classic LSB insertion
 *
 * @param image Source image
 * @param msg Bytes to insert
 * @param key Secret key
 * @param compress Use compression. No compression by default
 *
 * @return imagestego::ok on success
 */
IMAGESTEGO_EXPORTS imagestego::status insert(cv::Mat image, const std::string& msg,
        const std::string& key, imagestego::compressmode compress = imagestego::uncompressed);

/**
 * @brief A function that performs LSB extraction
 *
 * @param image Source image
 * @param msg Reference to extracted message
 * @param key Secret key
 * @param compress Use compression. No compression by default
 *
 * @return imagestego::ok on success
 */
// IMAGESTEGO_EXPORTS imagestego::status extract(cv::Mat image, std::string& msg,
        // const std::string& key, imagestego::compressmode compress = imagestego::uncompressed);


} // namespace lsb

} // namespace imagestego

#endif /* __IMAGESTEGO_LSB_HPP_INCLUDED__ */
