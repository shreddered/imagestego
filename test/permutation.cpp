#include "imagestego/utils/jpeg_permutation.hpp"

#include <iostream>

using imagestego::Permutation;


int main() {
    Permutation p(4, false);
    std::mt19937 gen;
    gen.seed(123);
    p.generate(gen);
    std::vector<short> v = {1, 2, 3, 4},
                       v1 = p * v;
    for (const auto& elem : v1)
        std::cout << elem << ' ';
    std::cout << std::endl;
    p.inverse();
    auto v2 = p * v1;
    for (const auto& elem : v2)
        std::cout << elem << ' ';
    std::cout << std::endl;
    return 0;
}
