#include "imagestego/utils/permutation.hpp"


namespace imagestego {

JpegPermutation::JpegPermutation() noexcept : JpegProcessor() {}

JpegPermutation::JpegPermutation(const std::string& src) noexcept : JpegProcessor(src) {}

JpegPermutation::~JpegPermutation() noexcept = default;

void JpegPermutation::seed(const std::string& _key) {
    uint32_t tmp[1];
    MurmurHash3_x86_32(_key.data(), _key.size(), 4991, tmp);
    key = tmp[0];
}

void JpegPermutation::process() const {
}

}
