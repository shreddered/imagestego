#include <imagestego/utils/avl_tree.hpp>

#include <cassert>
#include <vector>


template<class It1, class It2>
bool isEqual(It1 begin, It1 end, It2 _begin) {
    for (; begin != end; ++begin, ++_begin)
        if (*begin != *_begin)
            return false;
    return true;
}

int main() {
    auto v = {1, 3, 7, 1, 5, 8, 6};
    imagestego::AvlTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    assert(tree.size() == 6);
    auto result = {5, 3, 1, 7, 6, 8}; 
    assert(isEqual(tree.begin(), tree.end(), result.begin()));
    tree.clear();
    for (auto elem : {3, 7, 10, 6, 7, 0, 2, 4})
        tree.insert(elem);
    tree.printDfs();
    result = {3, 0, 2, 7, 6, 4, 10};
    assert(result.size() == tree.size());
    assert(isEqual(tree.begin(), tree.end(), result.begin())); 
    /*srand(time(nullptr));
    Route r(std::make_pair(500, 500));
    r.create(10);
    for (auto elem : r)
        std::cout << '[' << elem.first << "; " << elem.second << ']' << std::endl;
        */
    return 0;
}
