/*
 * This file is a part of imagestego library.
 *
 * Copyright (C) 2020 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "imagestego/core/route.hpp"
#include <random>


namespace imagestego {

RouteImpl::RouteImpl(const std::pair<int, int>& mapSize, std::mt19937& _gen) noexcept : AvlTree(), _mapSize(mapSize), gen(_gen) {}

void RouteImpl::create(const int& n) {
    while(size() != n) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

void RouteImpl::add() {
    auto sz = size();
    while(size() != sz + 1) {
        insert({gen() % _mapSize.first, gen() % _mapSize.second});
    }
}

void Route::setMapSize(const std::pair<int, int>& p) {
    _route->setMapSize(p);
}

Route::Route(const std::pair<int, int>& mapSize, std::mt19937& gen) : _route(new RouteImpl(mapSize, gen)) {}

Route::~Route() noexcept {
    if (_route)
        delete _route;
}

void Route::create(const int& n) {
    _route->create(n);
}

void Route::add() {
    _route->add();
}

typename Route::iterator Route::begin() {
    return _route->begin();
}

typename Route::iterator Route::end() {
    return _route->end();
}

bool Route::search(const std::pair<int, int>& p) {
    return _route->search(p);
}

} // namespace imagestego
