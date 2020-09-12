#include <imagestego/utils/avl_tree.hpp>

#include <vector>

#include <gtest/gtest.h>


template<class It1, class It2>
bool isEqual(It1 begin, It1 end, It2 _begin) {
    for (; begin != end; ++begin, ++_begin)
        if (*begin != *_begin)
            return false;
    return true;
}

TEST(TestAvl, AvlTree) {
    auto v = {1, 3, 7, 1, 5, 8, 6};
    imagestego::AvlTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    EXPECT_EQ(tree.size(), 6);
    assert(tree.size() == 6);
    auto result = {5, 3, 1, 7, 6, 8}; 
    EXPECT_TRUE(isEqual(tree.begin(), tree.end(), result.begin()));
    tree.clear();
    for (auto elem : {3, 7, 10, 6, 7, 0, 2, 4})
        tree.insert(elem);
    result = {3, 0, 2, 7, 6, 4, 10};
    EXPECT_EQ(result.size(), tree.size());
    EXPECT_TRUE(isEqual(tree.begin(), tree.end(), result.begin())); 
}
