#include <algorithms/lsb_stego.hpp>
#include <iostream>
#include <bitset>


cv::Point Lsb::generatePoint() const noexcept {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis1(0, image.rows - 1),
                                    dis2(0, image.cols - 1);
    return cv::Point(dis1(gen), dis2(gen));
}

Lsb::Lsb() noexcept : opts(LsbOptions::silly) {}

Lsb::Lsb(const LsbOptions& _opts) noexcept : opts(_opts) {}

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

void Lsb::createStegoContainer() const {
    if (opts == 0)
        __sillyLsbInsertion();
    cv::imwrite(outputFile, image);
}

void Lsb::__sillyLsbInsertion() const {
    std::size_t currentBitIndex = 0;
    msg.put('\0');
    std::cout << msg << std::endl;
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
                std::cout << std::bitset<1>(pixel.val[color]);
            }
        }
    }
}

std::string Lsb::extractMessage() {
    BitArray<unsigned char> arr;
    for (int row = 0; row != image.rows; ++row) {
        for (int col = 0; col != image.cols; ++col) {
            for (uint8_t color = 0; color != 3; ++color) {
                auto pixel = image.at<cv::Vec3b>(cv::Point(row, col));
                std::cout << std::bitset<1>(pixel.val[color]);
                bool b = (pixel.val[color] & 1u) != 0;
                arr.pushBack(b);
                if (arr.size() % 8 == 0 && arr.lastBlock() == 0) {
                    std::cout << std::endl;
                    return arr.toString();
                }
            }
        }
    }
}
