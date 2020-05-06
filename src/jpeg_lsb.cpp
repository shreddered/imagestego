#include "imagestego/algorithms/jpeg_lsb.hpp"


imagestego::JpegLsbEmbedder::JpegLsbEmbedder(const std::string& input, const std::string& _output) 
    : JpegProcessor(input), output(_output) {}

void imagestego::JpegLsbEmbedder::setImage(const std::string&) {}

void imagestego::JpegLsbEmbedder::setOutputName(const std::string& str) {
    output = str;
}

void imagestego::JpegLsbEmbedder::setMessage(const std::string& message) {
    msg = imagestego::BitArray<>(message);
}

void imagestego::JpegLsbEmbedder::setSecretKey(const std::string& _key) {
    key = imagestego::BitArray<>(_key);
}

void imagestego::JpegLsbEmbedder::createStegoContainer() const {
    process();
    writeTo(output);
}

void imagestego::JpegLsbEmbedder::process() const {
    if (key.empty())
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPROT
        setSecretKey(imagestego::keygen::generate());
#else
        throw imagestego::Exception(imagestego::Exception::Codes::NoKeyFound);
#endif
    msg.put(0);
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

imagestego::JpegLsbExtracter::JpegLsbExtracter(const std::string& image) : JpegProcessor(image) {}

void imagestego::JpegLsbExtracter::setImage(const std::string& str) {}

void imagestego::JpegLsbExtracter::setSecretKey(const std::string& _key) {
    key = imagestego::BitArray<>(_key);
}

std::string imagestego::JpegLsbExtracter::extractMessage() {
    imagestego::BitArray<> msg;
    std::size_t currentKeyIndex = 0;
    auto size = getChannelSize(0);
    for (int i = 0; i != size.first; ++i)
        for (int j = 0; j != size.second; ++j) {
            auto redBlock = getBlock(0, i, j),
                 greenBlock = getBlock(1, i, j),
                 blueBlock = getBlock(2, i, j);
            for (int k = 0; k != 64; ++k) {
                if ((redBlock[k] & 1) != key[currentKeyIndex]) {
                    if (blueBlock[k] != 0 && blueBlock[k] != 1)
                        msg.pushBack(blueBlock[k] & 1);
                }
                else { 
                    if (greenBlock[k] != 0 && greenBlock[k] != 1)
                        msg.pushBack(greenBlock[k] & 1);
                }
                currentKeyIndex = (currentKeyIndex + 1) % key.size();
                if (msg.size() && msg.size() % 8 == 0 && msg.lastBlock() == 0)
                    return msg.toString();
            }
        }
}

void imagestego::JpegLsbExtracter::process() const {}

imagestego::Algorithm imagestego::JpegLsbEmbedder::getAlgorithm() const noexcept {
    return imagestego::Algorithm::JpegLsb;
}

imagestego::Algorithm imagestego::JpegLsbExtracter::getAlgorithm() const noexcept {
    return imagestego::Algorithm::JpegLsb;
}
