#include "imagestego/utils/avl_tree.hpp"


namespace imagestego {

Route::RouteImpl::RouteImpl(const std::pair<int, int>& mapSize, std::mt19937& _gen) noexcept : AvlTree(), _mapSize(mapSize), gen(_gen) {}

void Route::RouteImpl::create(const int& n) {
    while(size() != n) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

void Route::RouteImpl::add() {
    auto sz = size();
    while(size() != sz + 1) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

Route::Route(const std::pair<int, int>& mapSize, std::mt19937& _gen) : route(new RouteImpl(mapSize, _gen))  {}

void Route::create(const int& n) {
    route->create(n);
}

void Route::add() {
    route->add();
}

typename Route::iterator Route::begin() {
    return route->begin();
}

typename Route::iterator Route::end() {
    return route->end(); 
}

Route::~Route() noexcept {
    delete route;
}

} // namespace imagestego
