#include <utils/avl_tree.hpp>



Route::Route(const std::pair<int, int>& mapSize) noexcept : AvlTree(), _mapSize(mapSize) {}


void Route::create(const int& n) {
    while(size() != n) {
        insert({rand() % _mapSize.first, rand() % _mapSize.second});
    }
}
