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

#ifndef __IMAGESTEGO_WAVELET_HPP_INCLUDED__
#define __IMAGESTEGO_WAVELET_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
#include "imagestego/core/interfaces.hpp"
#include "imagestego/wavelet/interfaces.hpp"
// c++ headers
#include <string>


namespace imagestego {

class WaveletEmbedderImpl;

class IMAGESTEGO_EXPORTS WaveletEmbedder : public AbstractStegoEmbedder {
public:

    /**
     * Constructs imagesteo::WaveletEmbedder instance.
     *
     * @param wavelet Wavelet transform.
     * @param encoder Data encoder.
     */
    explicit WaveletEmbedder(AbstractWavelet* wavelet, AbstractEncoder* encoder = nullptr);

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
    WaveletEmbedderImpl* pImpl;
}; // class WaveletEmbedder

} // namespace imagestego

#endif /* __IMAGESTEGO_WAVELET_HPP_INCLUDED__ */
