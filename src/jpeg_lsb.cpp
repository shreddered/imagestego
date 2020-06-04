#include "imagestego/algorithms/jpeg_lsb.hpp"


namespace imagestego {

JpegLsbEmbedder<void>::JpegLsbEmbedder() noexcept {}

JpegLsbEmbedder<void>::JpegLsbEmbedder(const std::string& input, const std::string& _output) : image(input), output(_output) {}

void JpegLsbEmbedder<void>::setImage(const std::string& im) {
    image.open(im);
}

void JpegLsbEmbedder<void>::setMessage(const std::string& _msg) {
    msg = BitArray<>(_msg);
}

void JpegLsbEmbedder<void>::setOutputName(const std::string& str) {
    output = str;
}

void JpegLsbEmbedder<void>::setSecretKey(const std::string& _key) {
    key = BitArray<>(_key);
}

Algorithm JpegLsbEmbedder<void>::getAlgorithm() const noexcept {
    return Algorithm::JpegLsb;
}

void JpegLsbEmbedder<void>::createStegoContainer() {
    if (key.empty()) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        auto s = keygen::generate();
        std::cout << "key = " << s << std::endl;
        setSecretKey(s);
#else
        throw Exception(Exception::Codes::NoKeyFound);
#endif
    }
    msg.put('\0');
    std::size_t idx = 0;
    for (int i = 0; i != image.rows && idx < msg.size(); ++i)
        for (int j = 0; j != image.cols && idx < msg.size(); ++j) {
            auto p = image.at(i, j);
            if (p[0] != 1 && p[0] != 0)
                if (msg[idx++])
                    p[0] |= 1;
                else
                    p[0] &= ~1;
        }
    image.writeTo(output);
}

JpegLsbExtracter<void>::JpegLsbExtracter() noexcept {}

JpegLsbExtracter<void>::JpegLsbExtracter(const std::string& _image) : image(_image) {}

void JpegLsbExtracter<void>::setImage(const std::string& str) {
    image.open(str);
}

void JpegLsbExtracter<void>::setSecretKey(const std::string& _key) {
    key = BitArray<>(_key);
}

Algorithm JpegLsbExtracter<void>::getAlgorithm() const noexcept {
    return Algorithm::JpegLsb;
}

std::string JpegLsbExtracter<void>::extractMessage() {
    if (key.empty())
        throw Exception(Exception::Codes::NoKeyFound);
    BitArray<unsigned char> arr;
    for (int i = 0; i != image.rows; ++i)
        for (int j = 0; j != image.cols; ++j) {
            auto p = image.at(i, j);
            if (p[0] != 1 && p[0] != 0)
                arr.pushBack((p[0] & 1) != 0);
            if (arr.size() && arr.size() % 8 == 0 && arr.lastBlock() == 0)
                return arr.toString();
        }
}

} // namespace imagestego
