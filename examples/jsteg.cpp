#include <algorithms/jpeg_lsb.hpp>
#include <iostream>


int main() {
    imagestego::JpegLsbEmbedder inserter("in.jpg", "out.jpg");
    inserter.setMessage("aa");
    inserter.createStegoContainer();
    imagestego::JpegLsbExtracter extracter("out.jpg");
    std::cout << extracter.extractMessage() << std::endl;
    return 0;
}
