#ifndef __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#include "imagestego/utils/jpeg_processor.hpp"
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
#   include "imagestego/keygen.hpp"
#endif
#ifdef IMAGESTEGO_ENABLE_FORMAT_CHECKNG
#   include "imagestego/utils/format_checker.hpp"
#endif
// c++
#include <string>
#include <vector>


namespace imagestego {

template<class EncoderType>
class JpegLsbEmbedder : public AbstractStegoEmbedder, private JpegProcessor {
public:
    explicit JpegLsbEmbedder() noexcept : JpegProcessor() {}
    explicit JpegLsbEmbedder(const std::string& input, const std::string& _output) 
        : JpegProcessor(input), output(_output) {} 
    void setImage(const std::string& src) override {
        JpegProcessor::read(src);
    }
    void setOutputName(const std::string& str) override {
        output = str;
    }
    void setOutputName(std::string&& str) {
        output = str;
    }
    void setMessage(const std::string& message) override {
        encoder.setMessage(message);
        msg = encoder.getEncodedMessage();
    }
    void setSecretKey(const std::string& _key) override {
       key = BitArray<>(_key); 
    }
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::JpegLsb;
    }
    void createStegoContainer() const override { 
        process();
        writeTo(output);
    }
private:
    EncoderType encoder;
    mutable BitArray<> msg, key;
    std::string output;
    void process() const override { 
        if (key.empty())
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPROT
            setSecretKey(keygen::generate());
#else
            throw Exception(Exception::Codes::NoKeyFound);
#endif
        BitArray<> arr;
        arr.pushBack(msg.size(), 32);
        msg = arr + msg;
        std::size_t currentMsgIndex = 0,
                    currentKeyIndex = 0;
        auto size = getChannelSize(0);
        for (int i = 0; i != size.first && currentMsgIndex != msg.size(); ++i) {
            for (int j = 0; j != size.second && currentMsgIndex != msg.size(); ++j) {
                auto redBlock = getBlock(0, i, j),
                     greenBlock = getBlock(1, i, j),
                     blueBlock = getBlock(2, i, j);
                for (int k = 0; k != 64; ++k) {
                    if ((redBlock[k] & 1) != key[currentKeyIndex]) { // blue case
                        if (blueBlock[k] != 0 && blueBlock[k] != 1) {
                            if (msg[currentMsgIndex++])
                                blueBlock[k] |= 1;
                            else
                                blueBlock[k] &= ~1;
                        }
                    }
                    else {
                        if (greenBlock[k] != 0 && greenBlock[k] != 1) {
                            if (msg[currentMsgIndex++])
                                greenBlock[k] |= 1;
                            else
                                greenBlock[k] &= ~1;
                        }
                    }
                    currentKeyIndex = (currentKeyIndex + 1) % key.size();
                }
            }
        }
    }
}; // class JpegLsbEmbedder

template<class DecoderType>
class JpegLsbExtracter : public AbstractStegoExtracter, private JpegProcessor {
public:
    explicit JpegLsbExtracter() noexcept : JpegProcessor() {}
    explicit JpegLsbExtracter(const std::string& image) : JpegProcessor(image) {}
    void setImage(const std::string& str) override {
        JpegProcessor::read(str);
    }
    void setSecretKey(const std::string& _key) override {
        key = BitArray<>(_key);
    }
    Algorithm getAlgorithm() const noexcept override {
        return Algorithm::JpegLsb;
    }
    std::string extractMessage() override {
        BitArray<> msg;
        std::size_t currentKeyIndex = 0,
                    msgSize = 0,
                    currentMsgIndex = 0;
        bool sizeKnown = false;
        auto size = getChannelSize(0);
        for (int i = 0; i != size.first; ++i)
            for (int j = 0; j != size.second; ++j) {
                auto redBlock = getBlock(0, i, j),
                     greenBlock = getBlock(1, i, j),
                     blueBlock = getBlock(2, i, j);
                for (int k = 0; k != 64; ++k) {
                    if ((redBlock[k] & 1) != key[currentKeyIndex]) {
                        if (blueBlock[k] != 0 && blueBlock[k] != 1) {
                            ++currentMsgIndex;
                            bool bit = (blueBlock[k] & 1) != 0;
                            if (currentMsgIndex > 32) {
                                sizeKnown = true;
                                msg.pushBack(bit);
                            }
                            else
                                msgSize |= bit << (32 - currentMsgIndex);
                        }
                    }
                    else { 
                        if (greenBlock[k] != 0 && greenBlock[k] != 1) {
                            ++currentMsgIndex;
                            bool bit = (greenBlock[k] & 1) != 0;
                            if (currentMsgIndex > 32) {
                                sizeKnown = true;
                                msg.pushBack(bit);
                            }
                            else
                                msgSize |= bit << (32 - currentMsgIndex);
                        }
                    }
                    currentKeyIndex = (currentKeyIndex + 1) % key.size();
                    if (sizeKnown && currentMsgIndex - 32 == msgSize) {
                        decoder.setMessage(msg);
                        return decoder.getDecodedMessage();
                    }
                }
            }
    }
private:
    DecoderType decoder;
    BitArray<> key;
    void process() const override {}
}; // class JpegLsbExtracter

template<>
class IMAGESTEGO_EXPORTS JpegLsbEmbedder<void> : public AbstractStegoEmbedder, private JpegProcessor {
public:
    explicit JpegLsbEmbedder() noexcept;
    explicit JpegLsbEmbedder(const std::string& input, const std::string& _output); 
    void setImage(const std::string&) override;
    void setOutputName(const std::string& str) override;
    void setMessage(const std::string& message) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
    void createStegoContainer() const override; 
private:
    mutable BitArray<> msg, key;
    std::string output;
    void process() const override;
}; // class JpegLsbStegoEmbedder

template<>
class IMAGESTEGO_EXPORTS JpegLsbExtracter<void> : public AbstractStegoExtracter, private JpegProcessor {
public:
    explicit JpegLsbExtracter() noexcept;
    explicit JpegLsbExtracter(const std::string& image);
    void setImage(const std::string& str) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
    std::string extractMessage() override;
private:
    BitArray<> key;
    void process() const override;
}; // class JpegLsbStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__ */
