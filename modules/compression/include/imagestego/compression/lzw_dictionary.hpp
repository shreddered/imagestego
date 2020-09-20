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
