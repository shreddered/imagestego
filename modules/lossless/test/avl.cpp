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

// imagestego headers
#include <avl_tree.hpp>
// c++ headers
#include <vector>
// gtest
#include <gtest/gtest.h>


template<class It1, class It2>
bool isEqual(It1 begin, It1 end, It2 _begin) {
    for (; begin != end; ++begin, ++_begin)
        if (*begin != *_begin)
            return false;
    return true;
}

TEST(Core, AvlTree1) {
    auto v = {1, 3, 7, 1, 5, 8, 6};
    imagestego::AvlTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    EXPECT_EQ(tree.size(), 6);
    assert(tree.size() == 6);
    std::vector<int> result = {5, 3, 1, 7, 6, 8};
    EXPECT_TRUE(isEqual(tree.begin(), tree.end(), result.begin()));
    tree.clear();
    for (auto elem : {3, 7, 10, 6, 7, 0, 2, 4})
        tree.insert(elem);
    result = {3, 0, 2, 7, 6, 4, 10};
    EXPECT_EQ(result.size(), tree.size());
    EXPECT_TRUE(isEqual(tree.begin(), tree.end(), result.begin()));
}

TEST(Core, AvlTree2) {
    std::vector<int> v = {5, 3, 4, 1};
    imagestego::AvlTree<int> tree(v.begin(), v.end());
    v = {4, 3, 1, 5};
    EXPECT_TRUE(isEqual(tree.begin(), tree.end(), v.begin()));
}
