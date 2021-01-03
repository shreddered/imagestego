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

#include "imagestego/compression/lzw_dictionary.hpp"

namespace imagestego {

Dictionary::Dictionary() noexcept : _codeTable(1 << maxBits) {
    for (unsigned int i = 0; i != 256; ++i) {
        _codeTable[i] = StringElement(i);
    }
}

Dictionary::~Dictionary() = default;

void Dictionary::clear() noexcept {
    for (unsigned int i = 0; i != 256; ++i) {
        _codeTable[i] = StringElement(i);
    }
    _newCode = 256;
}

std::string Dictionary::at(int index) {
    std::string tmp;
    while (index != -1) {
        tmp.insert(0, 1, char(_codeTable[index].value));
        index = _codeTable[index].prefixIndex;
    }
    return tmp;
}

int Dictionary::search(const StringElement& s) {
    if (s.prefixIndex == -1)
        return s.value;
    int index = _codeTable[s.prefixIndex].first;
    if (index == -1) {
        _codeTable[s.prefixIndex].first = _newCode;
        _codeTable[_newCode++] = s;
        return -1;
    } else { // perform search
        while (true) {
            uint8_t val = _codeTable[index].value;
            if (s.value == val)
                return index;
            if (s.value < val) {
                int left = _codeTable[index].left;
                if (left == -1) { // left insertion case
                    _codeTable[index].left = _newCode;
                    _codeTable[_newCode++] = s;
                    return -1;
                }
                index = left;
            } else {
                int right = _codeTable[index].right;
                if (right == -1) { // right insertion case
                    _codeTable[index].right = _newCode;
                    _codeTable[_newCode++] = s;
                    return -1;
                }
                index = right;
            }
        } // while(1)
    }
}

void Dictionary::add(const uint8_t& value, const int& prefixIndex) {
    _codeTable[_newCode++] = StringElement(value, prefixIndex);
}

} // namespace imagestego
