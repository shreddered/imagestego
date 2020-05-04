#ifndef __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__

// imagestego headers
#include <core.hpp>
#include <utils/avl_tree.hpp>
#include <utils/bitarray.hpp>
// jpeg utils
#include <utils/jpeg.hpp>
// c++
#include <string>
#include <vector>


class JpegLsbStegoInserter : public AbstractStegoInserter, protected JpegProcessor {
private:
    mutable BitArray<> msg, key;
    std::string output;
public:
    explicit JpegLsbStegoInserter(const std::string& input, const std::string& _output) 
       : JpegProcessor(input), output(_output) {}
    void setImage(const std::string&) override {}
    void setOutputName(const std::string& str) override {
        output = str;
    }
    void setMessage(const std::string& message) override {
        msg = BitArray<>(message);
    }
    void createStegoContainer() const override {
        process();
        writeTo(output);
    }
protected:
    void process() const override {
        msg.put(0);
        std::size_t currentMsgIndex = 0;
        for (int channel = 0; channel != 3 && currentMsgIndex != msg.size(); ++channel) {
            auto size = getChannelSize(channel);
            for (int i = 0; i != size.first && currentMsgIndex != msg.size(); ++i) {
                for (int j = 0; j != size.second && currentMsgIndex != msg.size(); ++j) {
                    auto block = getBlock(channel, i, j);
                    for (int k = 0; k != 64; ++k) {
                        if (block[k] != 0 && block[k] != 1) {
                            if (msg[currentMsgIndex++])
                                block[k] |= 1;
                            else
                                block[k] &= ~1;
                        }
                    }
                }
            }
        }
    }
}; // class JpegLsbStegoInserter

class JpegLsbStegoExtracter : public AbstractStegoExtracter, protected JpegProcessor {
private:
    BitArray<> key;
public:
    explicit JpegLsbStegoExtracter(const std::string& image) : JpegProcessor(image) {}
    void setImage(const std::string& str) override {}
    std::string extractMessage() override {
        BitArray<> msg;
        for (int channel = 0; channel != 3; ++channel) {
            auto size = getChannelSize(channel);
            for (int i = 0; i != size.first; ++i)
                for (int j = 0; j != size.second; ++j) {
                    auto block = getBlock(channel, i, j);
                    for (int k = 0; k != 64; ++k) {
                        if (block[k] != 0 && block[k] != 1)
                            msg.pushBack(block[k] & 1);
                        if (msg.size() && msg.size() % 8 == 0 && msg.lastBlock() == 0)
                            return msg.toString();
                    }
                }
        }
    }
protected:
    void process() const override {}
}; // class JpegLsbStegoExtracter

#endif /* __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__ */
