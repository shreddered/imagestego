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
