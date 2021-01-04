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
