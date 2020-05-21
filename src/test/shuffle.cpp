#include <imagestego/core.hpp>
#include <random>
#include <iostream>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::mt19937 gen;
    gen.seed(123);
    imagestego::shuffle(v.begin(), v.end(), gen); 
    for (auto& elem : v)
        std::cout << elem << std::endl;
    return 0;
}
