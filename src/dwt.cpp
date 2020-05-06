#include "imagestego/algorithms/dwt.hpp"


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

imagestego::DwtEmbedder::DwtEmbedder() noexcept {}

imagestego::DwtEmbedder::DwtEmbedder(const std::string& imageName, const std::string& output)
    : image(cv::imread(imageName)), outputFile(output) {}


void imagestego::DwtEmbedder::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void imagestego::DwtEmbedder::setOutputName(const std::string& filename) {
    outputFile = filename;
}

void imagestego::DwtEmbedder::setSecretKey(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), 4991, tmp);
    key = tmp[0];
}

void imagestego::DwtEmbedder::setMessage(const std::string& _msg) {
    msg = imagestego::BitArray<>(_msg);
}

void imagestego::DwtEmbedder::createStegoContainer() const {
    if (!key)
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
        setSecretKey(imagestego::keygen::generate());
#else
        throw imagestego::Exception(imagestego::Exception::Codes::NoKeyFound);
#endif
    msg.put(0);
    uint32_t sz = 4 * ceil(sqrt(msg.size())); 
    std::size_t currentMsgIndex = 0;
    auto arr = imagestego::BitArray<>::fromInt(sz);
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    imagestego::dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    for (int i = 0; i != dwtGreen.rows && currentMsgIndex != 32; ++i)
        for (int j = 0; j != dwtGreen.cols && currentMsgIndex != 32; ++j) { 
            if (arr[currentMsgIndex++])
                dwtGreen.at<short>(i, j) |= 1;
            else
                dwtGreen.at<short>(i, j) &= ~1;
        }
    cv::Mat tmp;
    imagestego::idwt(dwtGreen, tmp);
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
    imagestego::dwt(cropped, tmp);
    currentMsgIndex = 0;
    for (int i = tmp.rows >> 1; i != tmp.rows && currentMsgIndex != msg.size(); ++i) {
        for (int j = tmp.cols >> 1; j != tmp.cols && currentMsgIndex != msg.size(); ++j) {
            if (msg[currentMsgIndex++])
                tmp.at<short>(i, j) |= 1;
            else
                tmp.at<short>(i, j) &= ~1;
        }
    }
    imagestego::idwt(tmp, cropped);
    cropped.copyTo(planes[0](cv::Rect(x0, y0, sz, sz)));
    cv::merge(planes, image);
    cv::imwrite(outputFile, image);
}


imagestego::DwtExtracter::DwtExtracter() noexcept {}

imagestego::DwtExtracter::DwtExtracter(const std::string& imageName) : image(cv::imread(imageName)) {}

void imagestego::DwtExtracter::setImage(const std::string& imageName) {
    image = cv::imread(imageName);
}

void imagestego::DwtExtracter::setSecretKey(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), 4991, tmp);
    key = tmp[0];
}

std::string imagestego::DwtExtracter::extractMessage() {
    imagestego::BitArray<unsigned char> arr;
    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    cv::Mat dwtGreen;
    imagestego::dwt(planes[1](cv::Rect(planes[1].rows >> 1, planes[1].cols >> 1, 32, 1)), dwtGreen);
    imagestego::BitArray<uint32_t> arr1;
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
    imagestego::dwt(cropped, tmp);
    for (int i = tmp.rows >> 1; i != tmp.rows; ++i)
        for (int j = tmp.cols >> 1; j != tmp.cols; ++j) {
            arr.pushBack((tmp.at<short>(i, j) & 1) != 0);
            if (arr.size() && arr.size() % 8 == 0 && arr.lastBlock() == 0)
                return arr.toString();
        }
}

imagestego::Algorithm imagestego::DwtEmbedder::getAlgorithm() const noexcept {
    return imagestego::Algorithm::Dwt;
}

imagestego::Algorithm imagestego::DwtExtracter::getAlgorithm() const noexcept {
    return imagestego::Algorithm::Dwt;
}
