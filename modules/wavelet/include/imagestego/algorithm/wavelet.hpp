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

#ifndef __IMAGESTEGO_WAVELET_HPP_INCLUDED__
#define __IMAGESTEGO_WAVELET_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
#include "imagestego/core/interfaces.hpp"
#include "imagestego/wavelet/interfaces.hpp"
// c++ headers
#include <random>
#include <string>

namespace imagestego {

namespace impl {

class WaveletEmbedder;

class WaveletExtracter;

} // namespace impl

/**
 * @brief Class for wavelet-based embedding
 */
class IMAGESTEGO_EXPORTS WaveletEmbedder : public StegoEmbedder {
public:
    /**
     * Constructs imagesteo::WaveletEmbedder instance.
     *
     * @param wavelet Wavelet transform.
     * @param encoder Data encoder.
     */
    explicit WaveletEmbedder(Wavelet* wavelet,
                             Encoder* encoder = nullptr);

    /**
     * imagestego::WaveletEmbedder destructor.
     */
    virtual ~WaveletEmbedder() noexcept;

    /**
     * Setter for image.
     *
     * @param src Path to image.
     */
    void setImage(const std::string& src) override;

    /**
     * Setter for message.
     *
     * @param msg Message to be embedded.
     */
    void setMessage(const std::string& msg) override;

    /**
     * Setter for secret key.
     *
     * @param key Secret key.
     */
    void setSecretKey(const std::string& key) override;

    /**
     * Creates stego container.
     *
     * @param dst Path to new image.
     */
    void createStegoContainer(const std::string& dst) override;

private:
    impl::WaveletEmbedder* _pImpl;
}; // class WaveletEmbedder

class IMAGESTEGO_EXPORTS WaveletExtracter : public StegoExtracter {
public:
    /**
     * Constructs imagesteo::WaveletEmbedder instance.
     *
     * @param wavelet Wavelet transform.
     * @param encoder Data encoder.
     */
    explicit WaveletExtracter(Wavelet* wavelet,
                              Decoder* encoder = nullptr);

    /**
     * @brief Destructs WaveletExtracter
     */
    virtual ~WaveletExtracter() noexcept;

    /**
     * @brief Source image setter.
     */
    void setImage(const std::string& imageName) override;
    /**
     * @brief Setter for secret key.
     */
    void setSecretKey(const std::string& key) override;
    /**
     * @brief Function for extracting secret message.
     */
    std::string extractMessage() override;
private:
    impl::WaveletExtracter* _pImpl;
}; // class WaveletExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_WAVELET_HPP_INCLUDED__ */
