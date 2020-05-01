#include <algorithms/jpeg_lsb.hpp>
#include <iostream>


int main() {
    JpegLsbStegoInserter inserter("in.jpg", "out.jpg");
    inserter.setMessage("aa");
    inserter.createStegoContainer();
    JpegLsbStegoExtracter extracter("out.jpg");
    std::cout << extracter.extractMessage() << std::endl;
    return 0;
}
