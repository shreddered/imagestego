#ifndef __IMAGESTEGO_ROUTE_HPP_INCLUDED__
#define __IMAGESTEGO_ROUTE_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/avl_tree.hpp"
// c++ headers
#include <random>
#include <utility>


namespace imagestego {

class RouteImpl final : public AvlTree<std::pair<int, int>, PairComparator<int, int> > {
public:
    explicit RouteImpl(const std::pair<int, int>& mapSize, std::mt19937&) noexcept;
    template<class It>
    explicit RouteImpl(It begin, It end, std::mt19937& _gen) : AvlTree(begin, end), gen(_gen) {}
    inline void setMapSize(const std::pair<int, int>& mapSize) {
        _mapSize = mapSize;
    }
    void create(const int&);
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
    explicit Route(It begin, It end, std::mt19937& _gen);
    virtual ~Route() noexcept;
    void setMapSize(const std::pair<int, int>& mapSize);
    void create(const int&);
    void add();
    iterator begin();
    iterator end();
private:
    RouteImpl* _route;
}; // class Route

} // namespace imagestego

#endif /* __IMAGESTEGO_ROUTE_HPP_INCLUDED__ */
