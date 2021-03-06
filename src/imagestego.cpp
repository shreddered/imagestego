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

#include "imagestego/imagestego.hpp"

namespace imagestego {

void embedSecretMessage(AbstractStegoEmbedder* embedder, const std::string& image,
                        const std::string& output, const std::string& message,
                        const std::string& key) {
    std::unique_ptr<AbstractStegoEmbedder> ptr(embedder);
    embedder->setImage(image);
    embedder->setOutputName(output);
    embedder->setMessage(message);
    embedder->setSecretKey(key);
    embedder->createStegoContainer();
}

std::string extractSecretMessage(AbstractStegoExtracter* extracter,
                                 const std::string& image, const std::string& key) {
    std::unique_ptr<AbstractStegoExtracter> ptr(extracter);
    extracter->setImage(image);
    extracter->setSecretKey(key);
    return extracter->extractMessage();
}

} // namespace imagestego
