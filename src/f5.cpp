#include "imagestego/algorithms/f5.hpp"


namespace imagestego {

F5Embedder::F5Embedder() noexcept : JpegPermutation() {}

F5Embedder::F5Embedder(const std::string imagename, const std::string& output) 
    : JpegPermutation(imagename), outputFile(output) {}

void F5Embedder::setImage(const std::string& imagename) {
    JpegProcessor::read(imagename);
}

void F5Embedder::setOutputName(const std::string& dst) {
    outputFile = dst;
}

void F5Embedder::setMessage(const std::string& _msg) {
    msg = BitArray<>(_msg);
}

void F5Embedder::setSecretKey(const std::string& key) {
    JpegPermutation::generate(key);
}

void F5Embedder::createStegoContainer() const {
    JpegPermutation::process();
    JpegProcessor::writeTo(outputFile);
}

} // namespace imagestego
