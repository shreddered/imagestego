#ifndef __IMAGESTEGO_F3_HPP_INCLUDED__
#define __IMAGESTEGO_F3_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#include "imagestego/utils/jpeg_image.hpp"
// c++ headers
#include <string>


namespace imagestego {

template<class EncoderType>
class F3Embedder : public AbstractStegoEmbedder {
public:
    explicit F3Embedder() noexcept {}
    explicit F3Embedder(const std::string& input, const std::string& _output) : image(input), output(_output) {} 
    void setImage(const std::string& imageName) override {
        image.open(imageName);
    }
    void setOutputName(const std::string& filename) override {
        output = filename;
    }
    void setMessage(const std::string& _msg) override {
        encoder.setMessage(_msg);
        msg = encoder.getEncodedMessage();
    }
    void setSecretKey(const std::string& key) override {}
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::F3;
    }
    void createStegoContainer() const override {
        BitArray<> size;
        size.pushBack(msg.size(), 32);
        auto lsb = [](const short& value) -> bool {
            return (value & 1) != 0;
        };
        auto decrement = [](short& value) -> void {
            if (value > 0)
                --value;
            else
                ++value;
        };
        int i = 0, j = 0;
        std::size_t msgIndex = 0;
        bool flag = true;
        for (; i != image.rows; ++i) { 
            for (; j != image.cols && flag; ++j) {
                auto p = image.at(i, j);
                for (char k = 0; k != 3 && flag; ++k) {
                    if (p[k]) {
                        if (lsb(p[k]) != size[msgIndex]) {
                            if (p[k] == 1 || p[k] == -1)
                                p[k] = 0;
                            else {
                                decrement(p[k]);
                                ++msgIndex;
                            }
                        }
                        else
                            ++msgIndex;
                    }
                    flag = msgIndex != 32;
                }
            }
            if (j == image.cols)
                j = 0;
            if (!flag) {
                if (!j)
                    ++i;
                break;
            }
        }
        msgIndex = 0;
        for (; i != image.rows; ++i) {
            for (; j != image.cols; ++j) {
                auto p = image.at(i, j);
                for (char k = 0; k != 3; ++k) {
                    if (p[k]) {
                        if (lsb(p[k]) != msg[msgIndex]) {
                            if (p[k] == 1 || p[k] == -1)
                                p[k] = 0;
                            else {
                                decrement(p[k]);
                                ++msgIndex;
                            }
                        }
                        else
                            ++msgIndex;
                    }
                    if (msgIndex == msg.size()) {
                        image.writeTo(output);
                        return;
                    }
                }
            }
            j = 0;
        }
    }
private:
    std::string output;
    mutable JpegImage image;
    EncoderType encoder;
    BitArray<uint8_t> msg;
}; // class F3Embedder

template<class DecoderType>
class F3Extracter : public AbstractStegoExtracter {
public:
    explicit F3Extracter() noexcept {}
    explicit F3Extracter(const std::string& imageName) : image(imageName) {}
    void setImage(const std::string& imageName) override {
        image.open(imageName);
    }
    void setSecretKey(const std::string& key) override {}
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::F3;
    }
    std::string extractMessage() override {
        auto lsb = [](const short& value) -> bool {
            return (value & 1) != 0;
        };
        BitArray<uint32_t> size;
        BitArray<uint8_t> msg;
        bool flag = true;
        int i = 0, j = 0;
        for (; i != image.rows; ++i) {
            for (; j != image.cols && flag; ++j) {
                auto p = image.at(i, j);
                for (char k = 0; k != 3 && flag; ++k) {
                    if (p[k])
                        size.pushBack(lsb(p[k]));
                    flag = size.size() != 32; // lol
                }
            }
            if (j == image.cols)
                j = 0;
            if (!flag) {
                if (!j)
                    ++i;
                break;
            }
        }
        auto sz = size.getBlock(0);
        for (; i != image.rows; ++i) {
            for (; j != image.cols; ++j) {
                auto p = image.at(i, j);
                for (char k = 0; k != 3; ++k) {
                    if (p[k])
                        msg.pushBack(lsb(p[k]));
                    if (msg.size() == sz) {
                        decoder.setMessage(msg);
                        return decoder.getDecodedMessage();
                    }
                }
            }
        }
    }
private:
    JpegImage image;
    DecoderType decoder;
}; // class F3Extracter

template<>
class IMAGESTEGO_EXPORTS F3Embedder<void> : public AbstractStegoEmbedder {
public:
    explicit F3Embedder() noexcept;
    explicit F3Embedder(const std::string& input, const std::string& _output);
    void setImage(const std::string& imageName) override;
    void setOutputName(const std::string& filename) override;
    void setMessage(const std::string& msg) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override; 
    void createStegoContainer() const override;
private:
    mutable JpegImage image;
    std::string output;
    mutable BitArray<> msg;
}; // class F3Embedder

template<>
class IMAGESTEGO_EXPORTS F3Extracter<void> : public AbstractStegoExtracter {
public:
    explicit F3Extracter() noexcept;
    explicit F3Extracter(const std::string& input);
    void setImage(const std::string& imageName) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override; 
    std::string extractMessage() override;
private:
    JpegImage image;
}; // class F3Extracter

}; // namespace imagestego

#endif /* __IMAGESTEGO_F3_HPP_INCLUDED__ */
