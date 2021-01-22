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

#ifndef __LZW_DECODER_HPP_INCLUDED__
#define __LZW_DECODER_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <string>

namespace imagestego {

namespace impl {

class LzwDecoder;

} // namespace impl

/**
 * @brief Class implementing LZW decoding.
 */
class IMAGESTEGO_EXPORTS LzwDecoder : public Decoder {
public:
    /**
     * @brief Constructs empty LzwDecoder instance.
     */
    explicit LzwDecoder();
    /**
     * @brief Constructs LzwDecoder instance with given message.
     *
     * @param arr Message to be decoded.
     */
    explicit LzwDecoder(const BitArray& arr);
    /**
     * @brief Destructs LzwDecoder.
     */
    virtual ~LzwDecoder() noexcept;
    /**
     * @brief Setter for message to be decoded.
     *
     * @param str Message to be decoded.
     */
    void setMessage(const BitArray& arr) override;
    /**
     * @brief Getter for decoded message.
     *
     * @return Decoded message.
     */
    std::string getDecodedMessage() override;

private:
    impl::LzwDecoder* _decoder;
}; // class LzwDecoder

} // namespace imagestego

#endif /* __LZW_DECODER_HPP_INCLUDED__ */
