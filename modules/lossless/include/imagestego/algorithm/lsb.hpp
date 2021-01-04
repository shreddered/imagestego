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
#include "imagestego/core.hpp"
#include "imagestego/core/bitarray.hpp"
#include "imagestego/core/interfaces.hpp"
// c++ headers
#include <random>
#include <string>
#include <type_traits>


namespace imagestego {

class LsbEmbedderImpl;

class LsbExtracterImpl;

/**
 * Class for performing LSB-based embedding.
 */
class IMAGESTEGO_EXPORTS LsbEmbedder : public AbstractStegoEmbedder {
public:

    /**
     * Constructs embedder with given encoder.
     *
     * @param encoder Encoder which will process data.
     */
    explicit LsbEmbedder(AbstractEncoder* encoder = nullptr);

    /**
     * LsbEmbedder destructror.
     */
    virtual ~LsbEmbedder() noexcept;

    /**
     * Setter for source image.
     *
     * @param src Path to image.
     */
    void setImage(const std::string& src) override;

    /**
     * Setter for message.
     *
     * @param msg Message string.
     */
    void setMessage(const std::string& msg) override;

    /**
     * Setter for secret key.
     *
     * @param src Secret key string.
     */
    void setSecretKey(const std::string& key) override;

    /**
     * Performs embedding.
     *
     * @param dst Path to new image.
     */
    void createStegoContainer(const std::string& dst) override;
private:
    LsbEmbedderImpl* _embedder;
}; // class LsbEmbedder

class IMAGESTEGO_EXPORTS LsbExtracter : public AbstractStegoExtracter {
public:

    /**
     * Constructs extracter with given decoder.
     *
     * @param decoder Decoder of extracted data.
     */
    explicit LsbExtracter(AbstractDecoder* decoder = nullptr);

    /**
     * LsbExtracter destructror.
     */
    virtual ~LsbExtracter() noexcept;

    /**
     * Setter for stego container.
     *
     * @param src Path to image with stego message.
     */
    void setImage(const std::string& dst) override;

    /**
     * Sets secret key.
     *
     * @param key Secret key.
     */
    void setSecretKey(const std::string& key) override;

    /**
     * Extracts message from image.
     *
     * @return Extracted message.
     */
    std::string extractMessage() override;
private:
    LsbExtracterImpl* _extracter;
}; // class LsbExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_LSB_HPP_INCLUDED__ */
