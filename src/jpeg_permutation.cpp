#include "imagestego/utils/jpeg_permutation.hpp"


namespace imagestego {

Permutation::Permutation() noexcept {}

Permutation::Permutation(const int& dim, bool _dir) : mat(dim), dir(_dir) {}

void Permutation::seed(const std::string& key) {
    gen.seed(hash(key));
    generate();
}

void Permutation::inverse() {
    dir = !dir;
}

void Permutation::generate() {
    std::iota(mat.begin(), mat.end(), 0);
    shuffle(mat.begin(), mat.end(), gen);
}

} // namespace imagestego
