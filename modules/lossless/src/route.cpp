/*
 * This file is a part of imagestego library.
 *
 * Copyright (c) 2020-2021 Dmitry Kalinin <x.shreddered.x@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// imagestego headers
#include "route.hpp"
// c++ headers
#include <random>
#include <utility>


namespace imagestego {

RouteImpl::RouteImpl(const std::pair<int, int>& mapSize, std::mt19937& _gen) noexcept : AvlTree(), _mapSize(mapSize), gen(_gen) {}

void RouteImpl::create(std::size_t n) {
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

void Route::create(std::size_t n) {
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
