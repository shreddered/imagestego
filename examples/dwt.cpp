#include <algorithms/dwt.hpp>


int main() {
    cv::Mat m;
    imagestego::dwt(cv::imread("in.jpg"), m);
    return 0;
}
