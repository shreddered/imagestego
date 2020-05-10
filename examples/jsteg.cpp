#include <imagestego/algorithms/jpeg_lsb.hpp>

#include <iostream>


int main() {
    imagestego::JpegLsbEmbedder<void> inserter("in.jpg", "out.jpg");
    inserter.setMessage("aa");
    inserter.setSecretKey("bs");
    inserter.createStegoContainer();
    imagestego::JpegLsbExtracter<void> extracter("out.jpg");
    extracter.setSecretKey("bs");
    std::cout << extracter.extractMessage() << std::endl;
    return 0;
}
