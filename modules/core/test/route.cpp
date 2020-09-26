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
#include "imagestego/core/route.hpp"
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
