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

#ifndef __IMAGESTEGO_CORE_ROUTE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_ROUTE_HPP_INCLUDED__

// imagestego headers
#include "avl_tree.hpp"
// c++ headers
#include <random>
#include <utility>

namespace imagestego {

class RouteImpl final : public AvlTree<std::pair<int, int>, PairComparator<int, int>> {
public:
    explicit RouteImpl(const std::pair<int, int>& mapSize, std::mt19937&) noexcept;
    template<class It>
    explicit RouteImpl(It begin, It end, std::mt19937& _gen)
        : AvlTree(begin, end), gen(_gen) {}
    inline void setMapSize(const std::pair<int, int>& mapSize) { _mapSize = mapSize; }
    void create(std::size_t);
    void add();

private:
    std::pair<int, int> _mapSize;
    std::mt19937& gen;
}; // class RouteImpl

class IMAGESTEGO_EXPORTS Route {
public:
    typedef RouteImpl::iterator iterator;
    explicit Route(const std::pair<int, int>& mapSize, std::mt19937& gen);
    template<class It>
    explicit Route(It begin, It end, std::mt19937& _gen)
        : _route(new RouteImpl(begin, end, _gen)) {}
    virtual ~Route() noexcept;
    void setMapSize(const std::pair<int, int>& mapSize);
    void create(std::size_t n);
    void add();
    bool search(const std::pair<int, int>& p);
    iterator begin();
    iterator end();

private:
    RouteImpl* _route;
}; // class Route

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_ROUTE_HPP_INCLUDED__ */
