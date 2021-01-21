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
#include "imagestego/core/bitarray.hpp"
// c++ headers
#include <ostream>

namespace imagestego {

// BitArray
BitArray::BitArray(impl::BitArray* arr) noexcept : _arr(arr) {}

BitArray::BitArray() : _arr(new impl::BitArray()) {}

BitArray::BitArray(const std::string& str) : _arr(new impl::BitArray(str)) {}

BitArray::BitArray(std::size_t sz) : _arr(new impl::BitArray(sz)) {}

BitArray::~BitArray() noexcept {
    if (_arr)
        delete _arr;
}

BitArray::BitArray(const BitArray& other) : _arr(new impl::BitArray(*other._arr)) {}

BitArray& BitArray::operator=(const BitArray& other) {
    if (this != &other) {
        delete _arr;
        _arr = new impl::BitArray(*other._arr);
    }
    return *this;
}

BitArray::BitArray(BitArray&& other) noexcept : _arr(other._arr) { other._arr = nullptr; }

BitArray& BitArray::operator=(BitArray&& other) noexcept {
    delete _arr;
    _arr = other._arr;
    other._arr = nullptr;
    return *this;
}

BitArray BitArray::fromByteString(const std::string& str) {
    BitArray arr(new impl::BitArray(impl::BitArray::fromByteString(str)));
    return arr;
}

BitArray BitArray::fromInt(uint32_t num) {
    BitArray arr(new impl::BitArray(impl::BitArray::fromInt(num)));
    return arr;
}

typename impl::BitArray::reference BitArray::operator[](std::size_t i) {
    return _arr->operator[](i);
}

std::size_t BitArray::size() const noexcept { return _arr->size(); }

bool BitArray::operator[](std::size_t i) const { return _arr->operator[](i); }

void BitArray::pushBack(bool val) { _arr->pushBack(val); }

void BitArray::pushFront(std::size_t num) { _arr->pushFront(num); }

void BitArray::clear() { _arr->clear(); }

bool BitArray::empty() const noexcept { return _arr->empty(); }

typename BitArray::iterator BitArray::begin() { return _arr->begin(); }

typename BitArray::iterator BitArray::end() { return _arr->end(); }

std::string BitArray::toByteString() const { return _arr->toByteString(); }

std::string BitArray::toString() const { return _arr->toString(); }

std::size_t BitArray::toInt() const { return _arr->toInt(); }

void BitArray::put(std::size_t num, std::size_t n) { _arr->put(num, n); }

bool BitArray::operator==(const BitArray& other) { return *_arr == *other._arr; }

} // namespace imagestego

std::ostream& operator<<(std::ostream& os, const imagestego::BitArray& arr) {
    return os << (*arr._arr);
}
