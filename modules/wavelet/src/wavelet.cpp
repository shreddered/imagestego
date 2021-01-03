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

// imagestego headers
#include "imagestego/algorithm/wavelet.hpp"
#include "imagestego/core/bitarray.hpp"
// opencv headers
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace imagestego {

class WaveletEmbedderImpl {
public:
    explicit WaveletEmbedderImpl(AbstractWavelet* wavelet, AbstractEncoder* encoder)
        : _encoder(encoder), _wavelet(wavelet) {}
    virtual ~WaveletEmbedderImpl() noexcept {
        delete _wavelet;
        if (_encoder)
            delete _encoder;
    }
    void setImage(const std::string& src) { _wavelet->setMatrix(cv::imread(src)); }
    void setMessage(const std::string& msg) {
        if (_encoder) {
            _encoder->setMessage(msg);
        } else {
            arr = BitArray::fromByteString(msg);
        }
    }
    void setSecretKey(const std::string& key) {}

    void createStegoContainer(const std::string& dst) {}

private:
    AbstractEncoder* _encoder;
    AbstractWavelet* _wavelet;
    BitArray arr;
}; // class WaveletEmbedderImpl

WaveletEmbedder::WaveletEmbedder(AbstractWavelet* wavelet, AbstractEncoder* encoder)
    : pImpl(new WaveletEmbedderImpl(wavelet, encoder)) {}

WaveletEmbedder::~WaveletEmbedder() noexcept {
    if (pImpl)
        delete pImpl;
}

void WaveletEmbedder::setImage(const std::string& src) { pImpl->setImage(src); }

void WaveletEmbedder::setMessage(const std::string& msg) { pImpl->setMessage(msg); }

void WaveletEmbedder::setSecretKey(const std::string& key) { pImpl->setSecretKey(key); }

} // namespace imagestego
