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

#ifndef __LZW_ENCODER_HPP_INCLUDED__
#define __LZW_ENCODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <string>

namespace imagestego {

class LzwEncoderImpl;

class IMAGESTEGO_EXPORTS LzwEncoder : public AbstractEncoder {
public:
    explicit LzwEncoder();
    explicit LzwEncoder(const std::string& str);
    virtual ~LzwEncoder() noexcept;
    void setMessage(const std::string& str) override;
    BitArray getEncodedMessage() override;

private:
    LzwEncoderImpl* _encoder;
}; // class LzwEncoder

} // namespace imagestego

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
