#include "algorithms/jpeg_lsb.hpp"


imagestego::JpegLsbStegoInserter::JpegLsbStegoInserter(const std::string& input, const std::string& _output) 
    : JpegProcessor(input), output(_output) {}

void imagestego::JpegLsbStegoInserter::setImage(const std::string&) {}

void imagestego::JpegLsbStegoInserter::setOutputName(const std::string& str) {
    output = str;
}

void imagestego::JpegLsbStegoInserter::setMessage(const std::string& message) {
    msg = BitArray<>(message);
}

void imagestego::JpegLsbStegoInserter::createStegoContainer() const {
    process();
    writeTo(output);
}

void imagestego::JpegLsbStegoInserter::process() const {
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

imagestego::JpegLsbStegoExtracter::JpegLsbStegoExtracter(const std::string& image) : JpegProcessor(image) {}

void imagestego::JpegLsbStegoExtracter::setImage(const std::string& str) {}

std::string imagestego::JpegLsbStegoExtracter::extractMessage() {
    imagestego::BitArray<> msg;
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

void imagestego::JpegLsbStegoExtracter::process() const {}