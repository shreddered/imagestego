#include <algorithms/dwt.hpp>
#include <iostream>


void dwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_16S);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<unsigned char>(i, (j << 1)),
                 b = src.at<unsigned char>(i, (j << 1) + 1);
            dst.at<short>(i, j) = (a + b) / 2;
            dst.at<short>(i, j + x) = a - b;
        }
}

void idwt1D(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_8U);
    auto x = src.cols >> 1;
    for (int i = 0; i != src.rows; ++i)
        for (int j = 0; j != x; ++j) {
            auto a = src.at<short>(i, j),
                 b = src.at<short>(i, j + x);
            dst.at<char>(i, (j << 1)) = a + ((b + 1 < 0 && (b + 1) % 2) ? b : b + 1) / 2;
            dst.at<char>(i, (j << 1) + 1) = a - ((b < 0 && b % 2) ? b - 1 : b) / 2;
        }
}

void imagestego::dwt(const cv::Mat& src, cv::Mat& dst) {
    src.convertTo(dst, CV_8U);
    std::vector<cv::Mat> planes;
    cv::split(dst, planes);
    cv::Mat tmp;
    dwt1D(planes[0], tmp);
    idwt1D(tmp, planes[0]);
    cv::Mat m;
    cv::merge(planes, m);
    std::cout << cv::countNonZero(m != dst) << std::endl;
}
