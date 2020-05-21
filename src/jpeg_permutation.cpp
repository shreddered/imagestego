#include "imagestego/utils/jpeg_permutation.hpp"


namespace imagestego {

Permutation::Permutation() noexcept {}

Permutation::Permutation(const int& rows, const int& cols) {
}

void Permutation::seed(const std::string& key) {

}

void Permutation::generate(std::vector<int>& v) {
    std::iota(v.begin(), v.end(), 0);
    shuffle(v.begin(), v.end(), gen);
}

} // namespace imagestego
