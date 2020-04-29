#include <algorithms/lsb.hpp>

#include <bitset>
#include <iostream>


LsbInsertionError::LsbInsertionError(const std::string& msg) noexcept : std::runtime_error(msg) {}

LsbExtractionError::LsbExtractionError(const std::string& msg) noexcept : std::runtime_error(msg) {}

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

LsbInserter<void>::LsbInserter(const int& _opts) noexcept : opts(_opts) {}

LsbInserter<void>::LsbInserter(const std::string& imageName, const std::string& output, const int& _opts) : image(cv::imread(imageName)), outputFile(output), opts(_opts) {}

void LsbInserter<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void LsbInserter<void>::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void LsbInserter<void>::setMessage(const std::string& _msg) noexcept {
    msg = BitArray<>(_msg);
}

void LsbInserter<void>::setSecretKey(const std::string& _key) noexcept{
    key = BitArray<unsigned int>(_key);
}

void LsbInserter<void>::createStegoContainer() const {
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
            break;
    }
}

void LsbInserter<void>::__sillyLsbInsertion() const {
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

void LsbInserter<void>::__randomLsbInsertion(bool flag) const {
    if (key.empty())
        throw LsbInsertionError("No key found");
    seed();
    Route route(std::make_pair(image.cols, image.rows));
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
    Route r1(route.begin(), route.end());
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

LsbExtracter<void>::LsbExtracter(const std::string& imageName, const int& _opts) : image(cv::imread(imageName)), opts(_opts) {}

void LsbExtracter<void>::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void LsbExtracter<void>::setSecretKey(const std::string& _key) noexcept {
    key = BitArray<unsigned int>(_key);
}

std::string LsbExtracter<void>::extractMessage() { 
    switch(opts) {
        case 0:
            return __sillyLsbExtraction();
        case 1:
#if __cpluspus >= 201103L
            [[fallthrough]];
#endif
        case 3:
            return __randomLsbExtraction();
        default:
            return "";
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
        throw LsbExtractionError("No key found");
    seed();
    BitArray<uint8_t> arr;
    BitArray<std::size_t> tmp;
    Route r(std::make_pair(image.cols, image.rows));
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
    Route r1(r.begin(), r.end());
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