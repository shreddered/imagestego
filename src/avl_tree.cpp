#include "utils/avl_tree.hpp"


imagestego::Route::Route(const std::pair<int, int>& mapSize) noexcept : AvlTree(), _mapSize(mapSize) {}


void imagestego::Route::create(const int& n) {
    while(size() != n) {
        insert({rand() % _mapSize.first, rand() % _mapSize.second});
    }
}

void imagestego::Route::add() {
    auto sz = size();
    while(size() != sz + 1) {
        insert({rand() % _mapSize.first, rand() % _mapSize.second});
    }
}
