#include <algorithms/lsb_stego.hpp>

#include <bitset>
#include <iostream>


LsbInsertionError::LsbInsertionError(const std::string& msg) noexcept : std::runtime_error(msg) {}

cv::Point Lsb::generatePoint() const noexcept {
    auto row = rand() % image.rows,
         col = rand() % image.cols;
    std::cout << cv::Point(row, col) << std::endl;
    return cv::Point(row, col);
}

Lsb::Lsb() noexcept : opts(LsbOptions::silly) {}

Lsb::Lsb(const LsbOptions& _opts) noexcept : opts(_opts) {}

Lsb::Lsb(const std::string& imageName, const std::string& output, const LsbOptions& _opts) 
    : image(cv::imread(imageName)), outputFile(output), opts(_opts) {}

Lsb::Lsb(const std::string& output) : image(cv::imread(output)) {}

void Lsb::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void Lsb::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void Lsb::setMessage(const std::string& _msg) noexcept {
    this->msg = BitArray<>(_msg);
}

void Lsb::setStegoContainer(const std::string& filename) {
    this->image = cv::imread(filename);
}

void Lsb::setSecretKey(const std::string& _key) noexcept {
    this->key = BitArray<unsigned int>(_key);
}

void Lsb::createStegoContainer() const {
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
    //cv::imwrite(outputFile, image);
}

void Lsb::__sillyLsbInsertion() const {
    std::size_t currentBitIndex = 0;
    msg.put('\0');
    for (int row = 0; row != image.rows; ++row) {
        for (int col = 0; col != image.cols; ++col) {
            for (uint8_t color = 0; color != 3; ++color) {
                auto& pixel = image.at<cv::Vec3b>(cv::Point(row, col));
                if (currentBitIndex == msg.size()) 
                    return;
                if (msg[currentBitIndex++])
                    pixel.val[color] |= 1u;
                else
                    pixel.val[color] &= ~1u;
            }
        }
    }
}

std::string Lsb::extractMessage() {
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

std::string Lsb::__sillyLsbExtraction() const {
    BitArray<unsigned char> arr;
    for (int row = 0; row != image.rows; ++row) {
        for (int col = 0; col != image.cols; ++col) {
            for (uint8_t color = 0; color != 3; ++color) {
                auto pixel = image.at<cv::Vec3b>(cv::Point(row, col));
                bool b = (pixel.val[color] & 1u) != 0;
                arr.pushBack(b);
                if (arr.size() & 7u == 0 && arr.lastBlock() == 0) {
                    return arr.toString();
                }
            }
        }
    }
}

std::string Lsb::__randomLsbExtraction() const {
    if (!key.size())
        throw LsbInsertionError("No key found");
    seed();
    BitArray<uint8_t> arr;
    std::size_t currentKeyIndex = 0;
    while(1) {
        auto pixel = image.at<cv::Vec3b>(generatePoint());
        std::cout << pixel << std::endl;
        bool b = (pixel.val[0] & 1u) != 0;
        if (key[currentKeyIndex] != b) { // green case
            std::cout << "first\n";
            std::cout << "byte = " << std::bitset<8>(pixel.val[1]);
            std::cout << " value = " << ((pixel.val[1] & 1u) != 0) << std::endl;
            arr.pushBack((pixel.val[1] & 1u) != 0);
        }
        else {
            std::cout << "second\n";
            std::cout << "byte = " << std::bitset<8>(pixel.val[2]);
            std::cout << " value = " << ((pixel.val[2] & 1u) != 0) << std::endl;
            arr.pushBack((pixel.val[2] & 1u) != 0);
        }
        currentKeyIndex = (currentKeyIndex + 1) % key.size();
        if ((arr.size() & 7u) == 0 && arr.lastBlock() == 0)
            return arr.toString();
    }
}

void Lsb::__randomLsbInsertion(bool flag) const {
    if (!key.size())
        throw LsbInsertionError("No key found");
    cv::Mat _image;
    image.copyTo(_image);
    msg.put('\0');
    seed();
    for (int i = 0; i != msg.size(); ++i) {
        auto p = generatePoint();
        auto& pixel = image.at<cv::Vec3b>(p);
        // pixel's LSB = pixel.val[0] & 1
        bool bit = msg[i];
        if ((pixel.val[0] & 1u) != key[i % key.size()]) {
            if (flag)
                change(pixel[1]);
            std::cout << "first\n";
            std::cout << std::bitset<8>(pixel.val[1]) << " => ";
            if (bit)
                pixel.val[1] |= 1u;
            else
                pixel.val[1] &= ~1u;
            std::cout << std::bitset<8>(pixel.val[1]) << std::endl;
        }
        else {
            if (flag)
                change(pixel[2]);
            std::cout << "second\n";
            std::cout << std::bitset<8>(pixel.val[2]) << " => ";
            if (bit)
                pixel.val[2] |= 1u;
            else
                pixel.val[2] &= ~1u;
            std::cout << std::bitset<8>(pixel.val[2]) << " => ";
        }
        _image.at<cv::Vec3b>(p) = pixel;
        std::cout << _image.at<cv::Vec3b>(p) << std::endl;
    }
    cv::imwrite(outputFile, _image);
}

void Lsb::seed() const noexcept {
    srand(key.getBlock(0));
}

void Lsb::change(uint8_t& val) noexcept {
    if (val == 255)
        --val;
    else if (val == 0)
        ++val;
    else if (rand() % 2)
        ++val;
    else
        --val;
}
