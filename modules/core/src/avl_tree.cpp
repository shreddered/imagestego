#include "imagestego/core/avl_tree.hpp"


namespace imagestego {

Route::Route(const std::pair<int, int>& mapSize, std::mt19937& _gen) noexcept : AvlTree(), _mapSize(mapSize), gen(_gen) {}

void Route::create(const int& n) {
    while(size() != n) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

void Route::add() {
    auto sz = size();
    while(size() != sz + 1) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

} // namespace imagestego
