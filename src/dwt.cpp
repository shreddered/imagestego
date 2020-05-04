#include "algorithms/dwt.hpp"


void dwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.copyTo(dst);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<short>(i, (j << 1)),
                 b = src.at<short>(i, (j << 1) + 1);
            dst.at<short>(i, j) = (a + b) / 2;
            dst.at<short>(i, j + x) = a - b;
        }
}

void idwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.copyTo(dst);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<short>(i, j),
                 b = src.at<short>(i, j + x);
            dst.at<short>(i, (j << 1)) = a + ((b + 1 < 0 && (b + 1) % 2) ? b : b + 1) / 2;
            dst.at<short>(i, (j << 1) + 1) = a - ((b < 0 && b % 2) ? b - 1 : b) / 2;
        }
}

void imagestego::dwt(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_16S);
    cv::Mat tmp;
    dwt1D(dst, tmp);
    dwt1D(tmp.t(), dst);
    dst = dst.t();
}

void imagestego::idwt(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat tmp;
    idwt1D(src.t(), tmp);
    idwt1D(tmp.t(), dst);
    dst.convertTo(dst, CV_8U);
}

imagestego::DwtStegoInserter::DwtStegoInserter() noexcept {}

imagestego::DwtStegoInserter::DwtStegoInserter(const std::string& imageName, const std::string& output)
    : image(cv::imread(imageName)), outputFile(output) {}


void imagestego::DwtStegoInserter::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void imagestego::DwtStegoInserter::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void imagestego::DwtStegoInserter::setMessage(const std::string& _msg) {
    msg = imagestego::BitArray<>(_msg);
}

void imagestego::DwtStegoInserter::createStegoContainer() const {
    msg.put(0);
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat tmp;
    // DWT of blue channel
    imagestego::dwt(planes[0], tmp);
    // after that, perform embedding
    std::size_t currentMsgIndex = 0;
    for (int i = image.rows >> 1; i != image.rows && currentMsgIndex != msg.size(); ++i) {
        for (int j = image.cols >> 1; j != image.cols && currentMsgIndex != msg.size(); ++j) {
            if (msg[currentMsgIndex++])
                tmp.at<short>(i, j) |= 1;
            else
                tmp.at<short>(i, j) &= ~1;
        }
    }
    imagestego::idwt(tmp, planes[0]);
    cv::merge(planes, image);
    cv::imwrite(outputFile, image);
}


imagestego::DwtStegoExtracter::DwtStegoExtracter() noexcept {}

imagestego::DwtStegoExtracter::DwtStegoExtracter(const std::string& imageName) : image(cv::imread(imageName)) {}

void imagestego::DwtStegoExtracter::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

std::string imagestego::DwtStegoExtracter::extractMessage() {
    imagestego::BitArray<unsigned char> arr;
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat tmp;
    imagestego::dwt(planes[0], tmp);
    for (int i = image.rows >> 1; i != image.rows; ++i)
        for (int j = image.cols >> 1; j != image.cols; ++j) {
            arr.pushBack((tmp.at<short>(i, j) & 1) != 0);
            if (arr.size() && arr.size() % 8 ==0 && arr.lastBlock() == 0)
                return arr.toString();
        }
}
