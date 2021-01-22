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

namespace impl {

class LzwEncoder;

} // namespace impl

/**
 * @brief Class implementing LZW encoding.
 */
class IMAGESTEGO_EXPORTS LzwEncoder : public Encoder {
public:
    /**
     * @brief Constructs empty LzwEncoder instance.
     */
    explicit LzwEncoder();
    /**
     * @brief Constructs LzwEncoder instance with given message.
     *
     * @param arr Message to be encoded.
     */
    explicit LzwEncoder(const std::string& str);
    /**
     * @brief Destructs LzwEncoder.
     */
    virtual ~LzwEncoder() noexcept;
    /**
     * @brief Setter for message to be encoded.
     *
     * @param str Message to be decoded.
     */
    void setMessage(const std::string& str) override;
    /**
     * @brief Getter for encoded message.
     *
     * @return Encoded message.
     */
    BitArray getEncodedMessage() override;

private:
    impl::LzwEncoder* _encoder;
}; // class LzwEncoder

} // namespace imagestego

#endif /* __LZW_ENCODER_HPP_INCLUDED__ */
