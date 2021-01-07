#ifndef __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#include "imagestego/utils/jpeg_image.hpp"
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
#include "imagestego/keygen.hpp"
#include <iostream>
#endif
#ifdef IMAGESTEGO_ENABLE_FORMAT_CHECKNG
#include "imagestego/utils/format_checker.hpp"
#endif
// c++
#include <string>
#include <vector>

namespace imagestego {

#ifdef IMAGESTEGO_COMPRESSION_SUPPORT
template<typename EncoderType>
class JpegLsbEmbedder : public AbstractStegoEmbedder {
public:
    explicit JpegLsbEmbedder() noexcept {}
    explicit JpegLsbEmbedder(const std::string& input, const std::string& _output)
        : image(input), output(_output) {}
    void setImage(const std::string& img) override { image.open(img); }
    void setOutputName(const std::string& str) override { output = str; }
    void setMessage(const std::string& message) override {
        encoder.setMessage(message);
        msg = encoder.getEncodedMessage();
    }
    void setSecretKey(const std::string& _key) override {
        key = BitArray<unsigned char>(_key);
    }
    Algorithm getAlgorithm() const noexcept override { return Algorithm::JpegLsb; }
    void createStegoContainer() override {
        if (key.empty()) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
            auto s = keygen::generate();
            std::cout << "key = " << s << std::endl;
            setSecretKey(s);
#else
            throw Exception(Exception::Codes::NoKeyFound);
#endif
        }
        std::size_t keyIndex = 0, msgIndex = 0;
        BitArray<uint32_t> sizeStream;
        sizeStream.pushBack(msg.size(), 32);
        int i = 0, j = 0;
        for (; i != image.rows; ++i) {
            for (j = 0; j != image.cols && msgIndex != 32; ++j) {
                auto p = image.at(i, j);
                if ((p[0] & 1) != key[keyIndex]) {
                    if (p[1] != 1 && p[1]) {
                        if (sizeStream[msgIndex++])
                            p[1] |= 1;
                        else
                            p[1] &= ~1;
                    }
                } else {
                    if (p[2] != 1 && p[2]) {
                        if (sizeStream[msgIndex++])
                            p[2] |= 1;
                        else
                            p[2] &= ~1;
                    }
                }
                keyIndex = (keyIndex + 1) % key.size();
            }
            if (j == image.cols)
                j = 0;
            if (msgIndex == 32) {
                if (!j)
                    ++i;
                break;
            }
        }
        msgIndex = 0;
        for (; i != image.rows && msgIndex != msg.size(); ++i) {
            for (; j != image.cols && msgIndex != msg.size(); ++j) {
                auto p = image.at(i, j);
                if ((p[0] & 1) != key[keyIndex]) {
                    if (p[1] != 1 && p[1]) {
                        if (msg[msgIndex++])
                            p[1] |= 1;
                        else
                            p[1] &= ~1;
                    }
                } else {
                    if (p[2] != 1 && p[2]) {
                        if (msg[msgIndex++])
                            p[2] |= 1;
                        else
                            p[2] &= ~1;
                    }
                }
                keyIndex = (keyIndex + 1) % key.size();
            }
            j = 0; // well it's important
        }
        image.writeTo(output);
    }

private:
    BitArray<unsigned char> msg, key;
    EncoderType encoder;
    JpegImage image;
    std::string output;
}; // class JpegLsbEmbedder

template<typename DecoderType>
class JpegLsbExtracter : public AbstractStegoExtracter {
public:
    explicit JpegLsbExtracter() noexcept {}
    explicit JpegLsbExtracter(const std::string& _image) : image(_image) {}
    void setImage(const std::string& str) override { image.open(str); }
    void setSecretKey(const std::string& _key) override { key = BitArray<>(_key); }
    Algorithm getAlgorithm() const noexcept override { return Algorithm::JpegLsb; }
    std::string extractMessage() override {
        if (key.empty())
            throw Exception(Exception::Codes::NoKeyFound);
        std::size_t keyIndex = 0, msgIndex = 0;
        BitArray<uint32_t> sizeStream;
        BitArray<uint8_t> msg;
        int i = 0, j = 0;
        for (; i != image.rows; ++i) {
            for (; j != image.cols && sizeStream.size() != 32; ++j) {
                auto p = image.at(i, j);
                if (lsb(p[0]) != key[keyIndex]) {
                    if (p[1] != 1 && p[1])
                        sizeStream.pushBack(lsb(p[1]));
                } else {
                    if (p[2] != 1 && p[2])
                        sizeStream.pushBack(lsb(p[2]));
                }
                keyIndex = (keyIndex + 1) % key.size();
            }
            if (j == image.cols)
                j = 0;
            if (sizeStream.size() == 32) {
                if (!j)
                    ++i;
                break;
            }
        }
        std::size_t sz = sizeStream.getBlock(0);
        for (; i != image.rows && sz; ++i) {
            for (; j != image.cols && sz; ++j) {
                auto p = image.at(i, j);
                if (lsb(p[0]) != key[keyIndex]) {
                    if (p[1] != 1 && p[1]) {
                        msg.pushBack(lsb(p[1]));
                        --sz;
                    }
                } else {
                    if (p[2] != 1 && p[2]) {
                        msg.pushBack(lsb(p[2]));
                        --sz;
                    }
                }
                keyIndex = (keyIndex + 1) % key.size();
            }
        }
        decoder.setMessage(msg);
        return decoder.getDecodedMessage();
    }

private:
    BitArray<> key;
    JpegImage image;
    DecoderType decoder;
    static constexpr bool lsb(const short& value) { return (value & 1) != 0; }
}; // class JpegLsbExtracter
#else
template<class EncoderType>
class JpegLsbEmbedder;

template<class DecoderType>
class JpegLsbExtracter;
#endif /* IMAGESTEGO_COMPRESSION_SUPPORT */

template<>
class IMAGESTEGO_EXPORTS JpegLsbEmbedder<void> : public AbstractStegoEmbedder {
public:
    explicit JpegLsbEmbedder() noexcept;
    explicit JpegLsbEmbedder(const std::string& input, const std::string& _output);
    void setImage(const std::string&) override;
    void setOutputName(const std::string& str) override;
    void setMessage(const std::string& message) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
    void createStegoContainer() override;

private:
    BitArray<> msg, key;
    JpegImage image;
    std::string output;
}; // class JpegLsbEmbedder

template<>
class IMAGESTEGO_EXPORTS JpegLsbExtracter<void> : public AbstractStegoExtracter {
public:
    explicit JpegLsbExtracter() noexcept;
    explicit JpegLsbExtracter(const std::string& image);
    void setImage(const std::string& str) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
    std::string extractMessage() override;

private:
    BitArray<> key;
    JpegImage image;
}; // class JpegLsbExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__ */
