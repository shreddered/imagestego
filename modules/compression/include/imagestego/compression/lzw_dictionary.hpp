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

#ifndef __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__
#define __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
// c++ headers
#include <string>
#include <vector>


namespace imagestego {

struct StringElement final {
    int prefixIndex; // index of prefix string
    uint8_t value; // last byte value stored instead of string
    int first = -1; // index of first string which has *this as prefix
    int left = -1, right = -1; // indexes of string which have same prefix as *this
    explicit constexpr StringElement(const uint8_t val = 0, const int& prefix = -1) noexcept : prefixIndex(prefix), value(val) {}
}; // struct StringElement

class Dictionary {
public:
    static constexpr uint8_t maxBits = 12;
    explicit Dictionary() noexcept;
    int search(const StringElement& s);
    void clear() noexcept;
    inline unsigned int size() const noexcept {
        return newCode;
    }
    virtual ~Dictionary();
protected:
    std::string at(int code);
    void add(const uint8_t& value, const int& prefixIndex);
private:
    std::vector<StringElement> codeTable;
    unsigned int newCode = 256;
}; // class Dictionary

} // namespace imagestego

#endif /* __IMAGESTEGO_LZW_DICTIONARY_HPP_INCLUDED__ */
