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
#include <vector>
// gtest
#include <gtest/gtest.h>

using imagestego::Route;

template<typename It1, typename It2>
bool isEqual(It1 begin, It1 end, It2 _begin) {
    for (; begin != end; ++begin, ++_begin)
        if (*begin != *_begin)
            return false;
    return true;
}

TEST(Core, Route) {
    std::mt19937 gen;
    gen.seed(1);
    Route r({10, 15}, gen);
    r.create(3);
    std::vector<std::pair<int, int> > v = {{4, 8}, {3, 13}, {5, 14}};
    EXPECT_TRUE(isEqual(r.begin(), r.end(), v.begin()));
    r.add();
    v = {{4, 8}, {3, 13}, {1, 11}, {5, 14}};
    EXPECT_TRUE(isEqual(r.begin(), r.end(), v.begin()));
}
