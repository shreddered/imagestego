#include "imagestego/algorithms/lsb.hpp"


namespace imagestego {

#ifdef IMAGESTEGO_ENABLE_FORMAT_CHECKNG
void LsbEmbedder<void>::check() const {
    if (fmt.check(outputFile))
        throw Exception(Exception::Codes::NotJpegClass);
}
#endif

void __::change(uint8_t& val) noexcept {
    std::random_device rd;
    if (val == 255)
        --val;
    else if (val == 0)
        ++val;
    else if (rd() % 2)
        ++val;
    else
        --val;
}

LsbEmbedder<void>::LsbEmbedder(const int& _opts) noexcept : opts(_opts) {}

LsbEmbedder<void>::LsbEmbedder(const std::string& imageName, const std::string& output, const int& _opts) : image(cv::imread(imageName)), outputFile(output), opts(_opts) {}

void LsbEmbedder<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void LsbEmbedder<void>::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void LsbEmbedder<void>::setMessage(const std::string& _msg) noexcept {
    msg = BitArray<>(_msg);
}

void LsbEmbedder<void>::setSecretKey(const std::string& _key) noexcept{
    key = BitArray<unsigned int>(_key);
    seed(_key);
}

void LsbEmbedder<void>::seed(const std::string& str) const noexcept {
    uint32_t tmp[1];
    MurmurHash3_x86_32(str.data(), str.size(), 4991, tmp);
    gen.seed(*tmp); 
}

void LsbEmbedder<void>::createStegoContainer() const {
    switch(opts) {
        case 0:
            __sillyLsbInsertion();
            break;
        case 1:
            __randomLsbInsertion(0);
            break;
        case 3:
            __randomLsbInsertion(1);
            break;
        default:
            throw Exception(Exception::Codes::UnknownLsbMode);
    }
}

void LsbEmbedder<void>::__sillyLsbInsertion() const {
    std::size_t currentBitIndex = 0;
    msg.put('\0');
    for (int row = 0; row != image.rows && currentBitIndex != msg.size(); ++row) {
        for (int col = 0; col != image.cols && currentBitIndex != msg.size(); ++col) {
            for (uint8_t color = 0; color != 3; ++color) {
                auto pixel = image.at<cv::Vec3b>(row, col);
                if (msg[currentBitIndex++])
                    pixel.val[color] |= 1u;
                else
                    pixel.val[color] &= ~1u;
                image.at<cv::Vec3b>(row, col) = pixel;
            }
        }
    }
    cv::imwrite(outputFile, image);
}

void LsbEmbedder<void>::__randomLsbInsertion(bool flag) const {
#ifdef IMAGESTEGO_ENABLE_SPACE_CHECKING
    spaceCheck(32 + msg.size(), image, Algorithm::Lsb);
#endif
    if (key.empty()) {
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        auto s = keygen::generate();
        std::cout << s << std::endl;
        setSecretKey(s);
#else
        throw Exception(Exception::Codes::NoKeyFound);
#endif
    }
    Route route(std::make_pair(image.cols, image.rows), gen);
    route.create(32);
    auto it = route.begin();
    BitArray<std::size_t> tmp = BitArray<std::size_t>::fromInt(msg.size());
    std::size_t currentKeyIndex = 0;
    // writing size
    for (int i = 0; i != 32; ++i, ++it) {
        auto p = fromPair(it);
        auto& pixel = image.at<cv::Vec3b>(p.y, p.x);
        // pixel's LSB = pixel.val[0] & 1
        bool bit = tmp[i];
        if ((pixel.val[0] & 1u) != key[currentKeyIndex % key.size()]) {
            if (flag)
                __::change(pixel[1]);
            if (bit)
                pixel.val[1] |= 1u;
            else
                pixel.val[1] &= ~1u;
        }
        else {
            if (flag)
                __::change(pixel[2]);
            if (bit)
                pixel.val[2] |= 1u;
            else
                pixel.val[2] &= ~1u;
        }
        ++currentKeyIndex;
    }
    Route r1(route.begin(), route.end(), gen);
    r1.setMapSize(std::make_pair(image.cols, image.rows));
    r1.create(32 + msg.size());
    std::size_t i = 0;
    for (auto it = r1.begin(); it != r1.end(); ++it) {
        if (route.search(*it))
            continue;
        auto p = fromPair(it);
        auto& pixel = image.at<cv::Vec3b>(p.y, p.x);
        // pixel's LSB = pixel.val[0] & 1
        bool bit = msg[i];
        if ((pixel.val[0] & 1u) != key[currentKeyIndex % key.size()]) {
            if (flag)
                __::change(pixel[1]);
            if (bit)
                pixel.val[1] |= 1u;
            else
                pixel.val[1] &= ~1u;
        }
        else {
            if (flag)
                __::change(pixel[2]);
            if (bit)
                pixel.val[2] |= 1u;
            else
                pixel.val[2] &= ~1u;
        }
        ++currentKeyIndex;
        ++i;
    }
    cv::imwrite(outputFile, image);
}

LsbExtracter<void>::LsbExtracter(const int& _opts) noexcept : opts(_opts) {}

LsbExtracter<void>::LsbExtracter(const std::string& imageName, const int& _opts) 
    : image(cv::imread(imageName)), opts(_opts) {}

void LsbExtracter<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void LsbExtracter<void>::setSecretKey(const std::string& _key) noexcept {
    key = BitArray<unsigned int>(_key);
    seed(_key);
}

void LsbExtracter<void>::seed(const std::string& str) const noexcept {
    uint32_t tmp[1];
    MurmurHash3_x86_32(str.data(), str.size(), 4991, tmp);
    gen.seed(*tmp); 
}

std::string LsbExtracter<void>::extractMessage() { 
    switch(opts) {
        case 0:
            return __sillyLsbExtraction();
        case 1:
#if __cpluspus >= 201703L
            [[fallthrough]];
#endif
        case 3:
            return __randomLsbExtraction();
        default:
            throw Exception(Exception::Codes::UnknownLsbMode);
    }
}

std::string LsbExtracter<void>::__sillyLsbExtraction() const {
    BitArray<unsigned char> arr;
    for (int row = 0; row != image.rows; ++row) {
        for (int col = 0; col != image.cols; ++col) {
            for (uint8_t color = 0; color != 3; ++color) {
                const auto& pixel = image.at<cv::Vec3b>(row, col);
                bool b = (pixel.val[color] & 1u) != 0;
                arr.pushBack(b);
                if (arr.size() % 8 == 0 && arr.lastBlock() == 0) {
                    return arr.toString();
                }
            }
        }
    }
}

std::string LsbExtracter<void>::__randomLsbExtraction() const {
    if (key.empty())
        throw Exception(Exception::Codes::NoKeyFound);
    BitArray<uint8_t> arr;
    BitArray<std::size_t> tmp;
    Route r(std::make_pair(image.cols, image.rows), gen);
    r.create(32);
    std::size_t currentKeyIndex = 0;
    for (auto it = r.begin(); it != r.end(); ++it) {
        auto point = fromPair(it);
        auto pixel = image.at<cv::Vec3b>(point.y, point.x);
        bool b = (pixel.val[0] & 1u) != 0;
        if (key[currentKeyIndex] != b) // green case
            tmp.pushBack((pixel.val[1] & 1u) != 0);
        else 
            tmp.pushBack((pixel.val[2] & 1u) != 0);
        currentKeyIndex = (currentKeyIndex + 1) % key.size();
    }
    std::size_t size = tmp.getBlock(0);
    Route r1(r.begin(), r.end(), gen);
    r1.setMapSize(std::make_pair(image.cols, image.rows));
    r1.create(32 + size);
    for (auto it = r1.begin(); it != r1.end(); ++it) {
        if (r.search(*it))
            continue;
        auto point = fromPair(it);
        auto pixel = image.at<cv::Vec3b>(point.y, point.x);
        bool b = (pixel.val[0] & 1u) != 0;
        if (key[currentKeyIndex] != b) // green case
            arr.pushBack((pixel.val[1] & 1u) != 0);
        else
            arr.pushBack((pixel.val[2] & 1u) != 0);
        currentKeyIndex = (currentKeyIndex + 1) % key.size();
    }
    return arr.toString();
}

Algorithm LsbEmbedder<void>::getAlgorithm() const noexcept {
    return Algorithm::Lsb;
}

Algorithm LsbExtracter<void>::getAlgorithm() const noexcept {
    return Algorithm::Lsb;
}

} // namespace imagestego
