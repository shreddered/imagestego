#include "imagestego/algorithms/dwt.hpp"


namespace {

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

} // namespace

namespace imagestego {

void dwt(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_16S);
    cv::Mat tmp;
    dwt1D(dst, tmp);
    dwt1D(tmp.t(), dst);
    dst = dst.t();
}

void idwt(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat tmp;
    idwt1D(src.t(), tmp);
    idwt1D(tmp.t(), dst);
    dst.convertTo(dst, CV_8U);
}

DwtEmbedder::DwtEmbedder() noexcept {}

DwtEmbedder::DwtEmbedder(const std::string& imageName, const std::string& output)
    : image(cv::imread(imageName)), outputFile(output) {}


void DwtEmbedder::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void DwtEmbedder::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void DwtEmbedder::setSecretKey(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), 4991, tmp);
    key = tmp[0];
}

void DwtEmbedder::setMessage(const std::string& _msg) {
    msg = BitArray<>(_msg);
}

void DwtEmbedder::createStegoContainer() const {
    if (!key)
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        setSecretKey(keygen::generate());
#else
        throw Exception(Exception::Codes::NoKeyFound);
#endif
    msg.put(0);
    uint32_t sz = 4 * ceil(sqrt(msg.size())); 
    std::size_t currentMsgIndex = 0;
    auto arr = BitArray<>::fromInt(sz);
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    for (int i = 0; i != dwtGreen.rows && currentMsgIndex != 32; ++i)
        for (int j = 0; j != dwtGreen.cols && currentMsgIndex != 32; ++j) { 
            if (arr[currentMsgIndex++])
                dwtGreen.at<short>(i, j) |= 1;
            else
                dwtGreen.at<short>(i, j) &= ~1;
        }
    cv::Mat tmp;
    idwt(dwtGreen, tmp);
    tmp.copyTo(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)));
    // after that, perform embedding
    // seeding PRNG
    gen.seed(key);
    int x0 = 0, y0 = 0;
    do {
        y0 = gen() % image.rows;
        x0 = gen() % image.cols;
    } while (y0 > image.rows - sz || x0 > image.cols - sz);
    cv::Mat cropped = planes[0](cv::Rect(x0, y0, sz, sz));
    // DWT of blue channel
    dwt(cropped, tmp);
    currentMsgIndex = 0;
    for (int i = tmp.rows >> 1; i != tmp.rows && currentMsgIndex != msg.size(); ++i) {
        for (int j = tmp.cols >> 1; j != tmp.cols && currentMsgIndex != msg.size(); ++j) {
            if (msg[currentMsgIndex++])
                tmp.at<short>(i, j) |= 1;
            else
                tmp.at<short>(i, j) &= ~1;
        }
    }
    idwt(tmp, cropped);
    cropped.copyTo(planes[0](cv::Rect(x0, y0, sz, sz)));
    cv::merge(planes, image);
    cv::imwrite(outputFile, image);
}


DwtExtracter::DwtExtracter() noexcept {}

DwtExtracter::DwtExtracter(const std::string& imageName) : image(cv::imread(imageName)) {}

void DwtExtracter::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void DwtExtracter::setSecretKey(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), 4991, tmp);
    key = tmp[0];
}

std::string DwtExtracter::extractMessage() {
    BitArray<unsigned char> arr;
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    BitArray<uint32_t> arr1;
    for (int i = 0; i != dwtGreen.rows; ++i)
        for (int j = 0; j != dwtGreen.cols; ++j)
            arr1.pushBack((dwtGreen.at<short>(i, j) & 1) != 0);
    uint32_t sz = arr1.getBlock(0);
    // seeding PRNG
    gen.seed(key);
    int x0 = 0, y0 = 0;
    do {
        y0 = gen() % image.rows;
        x0 = gen() % image.cols;
    } while (y0 > image.rows - sz || x0 > image.cols - sz);
    cv::Mat cropped = planes[0](cv::Rect(x0, y0, sz, sz));
    // DWT of blue channel
    cv::Mat tmp;
    dwt(cropped, tmp);
    for (int i = tmp.rows >> 1; i != tmp.rows; ++i)
        for (int j = tmp.cols >> 1; j != tmp.cols; ++j) {
            arr.pushBack((tmp.at<short>(i, j) & 1) != 0);
            if (arr.size() && arr.size() % 8 == 0 && arr.lastBlock() == 0)
                return arr.toString();
        }
}

Algorithm DwtEmbedder::getAlgorithm() const noexcept {
    return Algorithm::Dwt;
}

Algorithm DwtExtracter::getAlgorithm() const noexcept {
    return Algorithm::Dwt;
}

} // namespace imagestego
