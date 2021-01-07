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
