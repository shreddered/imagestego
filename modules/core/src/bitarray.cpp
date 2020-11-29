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
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <ostream>


namespace imagestego {

// BitArray
BitArray::BitArray(BitArrayImpl* arr) noexcept : _arr(arr) {}

BitArray::BitArray() : _arr(new BitArrayImpl()) {}

BitArray::BitArray(const std::string& str) : _arr(new BitArrayImpl(str)) {}

BitArray::BitArray(std::size_t sz) : _arr(new BitArrayImpl(sz)) {}

BitArray::~BitArray() noexcept {
    if (_arr)
        delete _arr;
}

BitArray::BitArray(const BitArray& other) : _arr(new BitArrayImpl(*other._arr)) {}

BitArray& BitArray::operator =(const BitArray& other) {
    if (this != & other) {
        delete _arr;
        _arr = new BitArrayImpl(*other._arr);
    }
    return *this;
}

BitArray::BitArray(BitArray&& other) noexcept : _arr(other._arr) {
    other._arr = nullptr;
}

BitArray& BitArray::operator =(BitArray&& other) noexcept {
    delete _arr;
    _arr = other._arr;
    other._arr = nullptr;
    return *this;
}

BitArray BitArray::fromByteString(const std::string& str) {
    BitArray arr(new BitArrayImpl(BitArrayImpl::fromByteString(str)));
    return arr;
}

BitArray BitArray::fromInt(uint32_t num) {
    BitArray arr(new BitArrayImpl(BitArrayImpl::fromInt(num)));
    return arr;
}

typename BitArrayImpl::reference BitArray::operator[](std::size_t i) {
    return _arr->operator[](i);
}

std::size_t BitArray::size() const noexcept {
    return _arr->size();
}

bool BitArray::operator[](std::size_t i) const {
    return _arr->operator[](i);
}

void BitArray::pushBack(bool val) {
    _arr->pushBack(val);
}

void BitArray::pushFront(std::size_t num) {
    _arr->pushFront(num);
}

void BitArray::clear() {
    _arr->clear();
}

bool BitArray::empty() const noexcept {
    return _arr->empty();
}

typename BitArray::iterator BitArray::begin() {
    return _arr->begin();
}

typename BitArray::iterator BitArray::end() {
    return _arr->end();
}

std::string BitArray::toByteString() const {
    return _arr->toByteString();
}

std::string BitArray::toString() const {
    return _arr->toString();
}

std::size_t BitArray::toInt() const {
    return _arr->toInt();
}

void BitArray::put(std::size_t num, std::size_t n) {
    _arr->put(num, n);
}

bool BitArray::operator ==(const BitArray& other) {
    return *_arr == *other._arr;
}

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr) {
    return os << (*arr._arr);
}
