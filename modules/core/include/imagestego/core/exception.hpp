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

#ifndef __IMAGESTEGO_CORE_EXCEPTION_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_EXCEPTION_HPP_INCLUDED__

#if IMAGESTEGO_MSVC
#pragma warning(disable : 4257)
#endif

// imagestego headers
#include "imagestego/core/config.hpp"
// c++ headers
#include <stdexcept>

namespace imagestego {

class IMAGESTEGO_EXPORTS Exception : public std::exception {
public:
    explicit Exception(const int& _code) noexcept;
    const char* what() const noexcept override;
    enum Codes {
        NoSuchFile = 1,
        BigMessageSize = 1 << 1,
        NoKeyFound = 1 << 2,
        InternalError = 1 << 3,
        UnknownLsbMode = 1 << 4,
        NotJpegClass = 1 << 5
    };

private:
    int code;
};

} // namespace imagestego

#ifdef _MSC_VER
inline const char* imagestego::Exception::what() const noexcept {
    switch (code) {
        case Codes::NoSuchFile:
            return "No such file";
        case Codes::BigMessageSize:
            return "Cannot embed message: image is too small";
        case Codes::NoKeyFound:
            return "No key found";
        case Codes::InternalError:
            return "Internal error";
        case Codes::UnknownLsbMode:
            return "Unknown LSB mode";
        case Codes::NotJpegClass:
            return "Class 'LsbEmbedder' doesn't support JPEG pictures. Use "
                   "'JpegLsbEmbedder' "
                   "instead";
        default:
            return "Unknown Error";
    }
}
#endif

#if IMAGESTEGO_MSVC
#pragma warning(default : 4275)
#endif

#endif /* __IMAGESTEGO_CORE_EXCEPTION_HPP_INCLUDED__ */
