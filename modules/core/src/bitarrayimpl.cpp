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
#include "imagestego/core/bitarrayimpl.hpp"
#ifdef IMAGESTEGO_LITTLE_ENDIAN
#include "imagestego/core/intrinsic.hpp"
#endif
// c++ headers
#include <algorithm>
#include <cstring>
#include <ostream>

namespace imagestego {

namespace impl {

// BitReference
BitArray::BitReference::BitReference(typename BitArray::BlockType& block,
                                         std::size_t pos) noexcept
    : _block(block), _mask(1 << (bitsPerBlock - pos - 1)) {}

BitArray::BitReference::operator bool() const { return (_block & _mask) != 0; }

BitArray::BitReference& BitArray::BitReference::operator=(bool val) noexcept {
    if (val)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

BitArray::BitReference&
BitArray::BitReference::operator=(const BitReference& other) noexcept {
    if (other)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

bool BitArray::BitReference::operator==(
    const BitArray::BitReference& other) noexcept {
    return bool(other) == bool(*this);
}

// BitIterator
BitArray::BitIterator::BitIterator(BitArray* owner, std::size_t pos) noexcept
    : _owner(owner), _pos(pos) {}

BitArray::BitReference BitArray::BitIterator::operator*() {
    return _owner->operator[](_pos);
}

BitArray::BitIterator& BitArray::BitIterator::operator++() {
    ++_pos;
    return *this;
}

BitArray::BitIterator BitArray::BitIterator::operator++(int) {
    BitIterator it = *this;
    operator++();
    return it;
}

BitArray::BitIterator& BitArray::BitIterator::operator--() {
    --_pos;
    return *this;
}

BitArray::BitIterator BitArray::BitIterator::operator--(int) {
    BitIterator it = *this;
    operator--();
    return it;
}

bool BitArray::BitIterator::operator==(const BitIterator& other) noexcept {
    return _owner == other._owner && _pos == other._pos;
}

bool BitArray::BitIterator::operator!=(const BitIterator& other) noexcept {
    return !(*this == other);
}

// BitArray
BitArray::BitArray() noexcept : _blocks(), _sz(0) {}

BitArray::BitArray(std::size_t sz) : _blocks(numberOfBlocks(sz), 0), _sz(sz) {}

BitArray::BitArray(const std::string& str)
    : _blocks(numberOfBlocks(str.size())), _sz(str.size()) {
    for (std::size_t i = 0; i != _sz; ++i) {
        BlockType& block = _blocks[blockIndex(i)];
        BlockType mask = 1 << (bitsPerBlock - bitIndex(i) - 1);
        if (str[i] - '0' == 0)
            block &= ~mask;
        else
            block |= mask;
    }
}

BitArray::BitReference BitArray::operator[](std::size_t i) {
    return BitReference(_blocks[blockIndex(i)], bitIndex(i));
}

bool BitArray::operator[](std::size_t i) const {
    return (_blocks[blockIndex(i)] & (1 << (bitsPerBlock - bitIndex(i) - 1))) != 0;
}

std::size_t BitArray::size() const noexcept { return _sz; }

BitArray BitArray::fromByteString(std::string str) {
    BitArray arr(str.size() * CHAR_BIT);
    if (str.size() % sizeof(BlockType) != 0)
        str.append(sizeof(BlockType) - str.size() % sizeof(BlockType), '\0');
    memcpy(&arr._blocks[0], str.data(), str.size() * sizeof(char));
#ifdef IMAGESTEGO_LITTLE_ENDIAN
    std::for_each(arr._blocks.begin(), arr._blocks.end(),
                  [](uint32_t& value) { value = bswap(value); });
#endif
    return arr;
}

BitArray BitArray::fromInt(uint32_t num) {
    BitArray arr;
    arr._blocks.push_back(num);
    arr._sz = sizeof(num) * CHAR_BIT;
    return arr;
}

void BitArray::pushBack(bool val) {
    if (_sz / CHAR_BIT == _blocks.size() * sizeof(BlockType) && _sz % CHAR_BIT == 0)
        _blocks.push_back(0);
    BitReference(_blocks[blockIndex(_sz)], bitIndex(_sz)) = val;
    ++_sz;
}

void BitArray::put(std::size_t num, std::size_t n) {
    for (; n != 0; --n) {
        const std::size_t offset = n - 1;
        pushBack((num & (1 << offset)) != 0);
    }
}

void BitArray::pushFront(std::size_t num) {
    _blocks.insert(_blocks.begin(), num);
    _sz += 32;
}

void BitArray::clear() {
    _blocks.clear();
    _sz = 0;
}

bool BitArray::empty() const noexcept { return _sz == 0; }

typename BitArray::iterator BitArray::begin() { return BitIterator(this, 0); }

typename BitArray::iterator BitArray::end() { return BitIterator(this, _sz); }

std::string BitArray::toByteString() const {
    std::string str(_sz / CHAR_BIT, '\0');
#if IMAGESTEGO_LITTLE_ENDIAN
    auto tmp = _blocks;
    std::for_each(tmp.begin(), tmp.end(), [](uint32_t& value) { value = bswap(value); });
    memcpy(&str[0], &tmp[0], tmp.size() * sizeof(BlockType));
#else
    memcpy(&str[0], &_blocks[0], _blocks.size() * sizeof(BlockType));
#endif
    return str;
}

std::string BitArray::toString() const {
    std::string str;
    str.reserve(_sz);
    for (std::size_t i = 0; i != _sz; ++i)
        str.push_back((operator[](i)) ? '1' : '0');
    return str;
}

std::size_t BitArray::toInt() const { return (_blocks.empty()) ? 0 : _blocks[0]; }

bool BitArray::operator==(const BitArray& other) {
    return _sz == other._sz &&
           std::equal(_blocks.begin(), _blocks.end(), other._blocks.begin());
}

} // namespace impl

} // namespace imagestego

std::ostream& operator<<(std::ostream& os, const imagestego::impl::BitArray& arr) {
    for (std::size_t i = 0; i != arr.size(); ++i) {
        os << arr[i];
    }
    return os;
}
