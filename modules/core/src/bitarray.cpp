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
#include "imagestego/core/config.hpp"
#ifdef IMAGESTEGO_LITTLE_ENDIAN
#   include "imagestego/core/intrinsic.hpp"
#endif
// c++ headers
#include <algorithm>
#include <cstring>
#include <ostream>


namespace imagestego {

// BitReference
BitArrayImpl::BitReference::BitReference(typename BitArrayImpl::BlockType& block, imagestego::size_t pos) noexcept
    : _block(block), _mask(1 << (bitsPerBlock - pos - 1)) {}

BitArrayImpl::BitReference::operator bool() const {
    return (_block & _mask) != 0;
}

BitArrayImpl::BitReference& BitArrayImpl::BitReference::operator =(bool val) noexcept {
    if (val)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

BitArrayImpl::BitReference& BitArrayImpl::BitReference::operator =(const BitReference& other) noexcept {
    if (other)
        _block |= _mask;
    else
        _block &= ~_mask;
    return *this;
}

bool BitArrayImpl::BitReference::operator ==(const BitArrayImpl::BitReference& other) noexcept {
    return bool(other) == bool(*this);
}

// BitIterator
BitArrayImpl::BitIterator::BitIterator(BitArrayImpl* owner, imagestego::size_t pos) noexcept
    : _owner(owner), _pos(pos) {}

BitArrayImpl::BitReference BitArrayImpl::BitIterator::operator *() {
    return (*_owner)[_pos];
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator ++() {
    ++_pos;
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator ++(int) {
    BitIterator it = *this;
    operator ++();
    return it;
}

BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator --() {
    --_pos;
    return *this;
}

BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator --(int) {
    BitIterator it = *this;
    operator --();
    return it;
}

// BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator +=(imagestego::size_t i) {
    // return *this;
// }

// BitArrayImpl::BitIterator& BitArrayImpl::BitIterator::operator -=(imagestego::size_t i) {
    // _ref._block = *(&_ref._block - i);
    // return *this;
// }

// BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator +(imagestego::size_t i) {
    // BitIterator it = *this;
    // return it += i;
// }

// BitArrayImpl::BitIterator BitArrayImpl::BitIterator::operator -(imagestego::size_t i) {
    // BitIterator it = *this;
    // return it -= i;
// }

bool BitArrayImpl::BitIterator::operator==(const BitIterator& other) noexcept {
    return _owner == other._owner && _pos == other._pos;
}

bool BitArrayImpl::BitIterator::operator!=(const BitIterator& other) noexcept {
    return !(*this == other);
}

// BitArrayImpl
BitArrayImpl::BitArrayImpl() noexcept : _blocks(), _sz(0) {}

BitArrayImpl::BitArrayImpl(imagestego::size_t sz) : _blocks(numberOfBlocks(sz), 0), _sz(sz) {}

BitArrayImpl::BitArrayImpl(const std::string& str) : _blocks(numberOfBlocks(str.size())), _sz(str.size()) {
    for (imagestego::size_t i = 0; i != _sz; ++i) {
        BlockType& block = _blocks[blockIndex(i)];
        BlockType mask = 1 << (bitsPerBlock - bitIndex(i) - 1);
        if (str[i] - '0' == 0)
            block &= ~mask;
        else
            block |= mask;
    }
}

BitArrayImpl::BitReference BitArrayImpl::operator [](imagestego::size_t i) {
    return BitReference(_blocks[blockIndex(i)], bitIndex(i));
}

bool BitArrayImpl::operator[](imagestego::size_t i) const {
    return (_blocks[blockIndex(i)] & (1 << (bitsPerBlock - bitIndex(i) - 1))) != 0;
}

imagestego::size_t BitArrayImpl::size() const noexcept {
    return _sz;
}

BitArrayImpl BitArrayImpl::fromByteString(std::string str) {
    BitArrayImpl arr(str.size() * CHAR_BIT);
    if (str.size() % sizeof(BlockType) != 0)
        str.append(sizeof(BlockType) - str.size() % sizeof(BlockType), '\0');
    memcpy(&arr._blocks[0], str.data(), str.size() * sizeof(char));
#ifdef IMAGESTEGO_LITTLE_ENDIAN
    std::for_each(arr._blocks.begin(), arr._blocks.end(), [](uint32_t& value) {
        value = bswap(value);
    });
#endif
    return arr;
}

BitArrayImpl BitArrayImpl::fromInt(imagestego::size_t num) {
    BitArrayImpl arr;
    arr._blocks.push_back(num);
    arr._sz = sizeof(num) * CHAR_BIT;
    return arr;
}

void BitArrayImpl::pushBack(bool val) {
    if (_sz / CHAR_BIT == _blocks.size() * sizeof(BlockType) && _sz % CHAR_BIT == 0)
        _blocks.push_back(0);
    BitReference(_blocks[blockIndex(_sz)], bitIndex(_sz)) = val;
    ++_sz;
}

void BitArrayImpl::put(int num, imagestego::size_t n) {
    for(; n != 0; --n) {
        const imagestego::size_t offset = n - 1;
        pushBack((num & (1 << offset)) != 0);
    }
}

void BitArrayImpl::put(imagestego::size_t num, imagestego::size_t n) {
    for(; n != 0; --n) {
        const imagestego::size_t offset = n - 1;
        pushBack((num & (1 << offset)) != 0);
    }
}

void BitArrayImpl::pushFront(imagestego::size_t num) {
    _blocks.insert(_blocks.begin(), num);
    _sz += 32;
}

void BitArrayImpl::clear() {
    _blocks.clear();
    _sz = 0;
}

bool BitArrayImpl::empty() const noexcept {
    return _sz == 0;
}

typename BitArrayImpl::iterator BitArrayImpl::begin() {
    return BitIterator(this, 0);
}

typename BitArrayImpl::iterator BitArrayImpl::end() {
    return BitIterator(this, _sz);
}

std::string BitArrayImpl::toByteString() const {
    std::string str(_sz / CHAR_BIT, '\0');
#if IMAGESTEGO_LITTLE_ENDIAN
    auto tmp = _blocks;
    std::for_each(tmp.begin(), tmp.end(), [](uint32_t& value) {
        value = bswap(value);
    });
    memcpy(&str[0], &tmp[0], tmp.size() * sizeof(BlockType));
#else
    memcpy(&str[0], &_blocks[0], _blocks.size() * sizeof(BlockType));
#endif
    return str;
}

std::string BitArrayImpl::toString() const {
    std::string str;
    str.reserve(_sz);
    for (imagestego::size_t i = 0; i != _sz; ++i)
        str.push_back((operator [](i)) ? '1' : '0');
    return str;
}

bool BitArrayImpl::operator ==(const BitArrayImpl& other) {
    return _sz == other._sz && std::equal(_blocks.begin(), _blocks.end(), other._blocks.begin());
}

// BitArray
BitArray::BitArray(BitArrayImpl* arr) noexcept : _arr(arr) {}

BitArray::BitArray() : _arr(new BitArrayImpl()) {}

BitArray::BitArray(const std::string& str) : _arr(new BitArrayImpl(str)) {}

BitArray::BitArray(imagestego::size_t sz) : _arr(new BitArrayImpl(sz)) {}

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

BitArray BitArray::fromInt(imagestego::size_t num) {
    BitArray arr(new BitArrayImpl(BitArrayImpl::fromInt(num)));
    return arr;
}

typename BitArrayImpl::reference BitArray::operator[](imagestego::size_t i) {
    return (*_arr)[i];
}

imagestego::size_t BitArray::size() const noexcept {
    return _arr->size();
}

bool BitArray::operator[](imagestego::size_t i) const {
    return (*_arr)[i];
}

void BitArray::pushBack(bool val) {
    _arr->pushBack(val);
}

void BitArray::pushFront(imagestego::size_t num) {
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

void BitArray::put(int num, imagestego::size_t n) {
    _arr->put(num, n);
}

void BitArray::put(imagestego::size_t num, imagestego::size_t n) {
    _arr->put(num, n);
}

bool BitArray::operator ==(const BitArray& other) {
    return *_arr == *other._arr;
}

} // namespace imagestego

std::ostream& operator <<(std::ostream& os, const imagestego::BitArrayImpl& arr) {
    for (imagestego::size_t i = 0; i != arr.size(); ++i) {
        os << arr[i];
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const imagestego::BitArray& arr) {
    return os << (*arr._arr);
}
