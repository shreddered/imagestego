#include "imagestego/utils/avl_tree.hpp"


imagestego::Route::Route(const std::pair<int, int>& mapSize, std::mt19937& _gen) noexcept : AvlTree(), _mapSize(mapSize), gen(_gen) {}

void imagestego::Route::create(const int& n) {
    while(size() != n) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

void imagestego::Route::add() {
    auto sz = size();
    while(size() != sz + 1) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}
