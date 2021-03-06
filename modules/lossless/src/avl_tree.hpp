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

#ifndef __IMAGESTEGO_CORE_AVL_TREE_HPP_INCLUDED__
#define __IMAGESTEGO_CORE_AVL_TREE_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core/config.hpp"
// c++ headers
#include <algorithm>
#include <functional>
#ifdef IMAGESTEG0_DEBUG
#include <iostream>
#endif
#include <random>
#include <utility>

namespace imagestego {

template<typename T, class Comp = std::less<T>>
class AvlTree {
    class Iterator;

public:
    typedef T value_type;
    typedef T& const_reference;
    typedef Iterator iterator;
    typedef const Iterator const_iterator;
    explicit AvlTree() noexcept {}
    template<class It>
    explicit AvlTree(It begin, It end) {
        for (auto it = begin; it != end; ++it)
            insert(*it);
    }
    AvlTree(const AvlTree<T, Comp>& other) : _size(other._size) {
        copy(other.root, root);
    }
    AvlTree(AvlTree<T, Comp>&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
    }
    virtual ~AvlTree() noexcept {
        if (!isEmpty())
            delete root;
    }
    AvlTree<T, Comp>& operator=(AvlTree<T, Comp>&& other) noexcept {
        root = other.root;
        _size = other._size;
        other.root = nullptr;
        return *this;
    }
    AvlTree<T, Comp>& operator=(const AvlTree<T, Comp>& other) {
        if (this != &other)
            copy(other.root, root);
        return *this;
    }
    void insert(const T& data) noexcept { root = insertImpl(root, data); }
    inline bool isEmpty() const noexcept { return root == nullptr; }
    bool search(const T& data) const noexcept { return searchImpl(root, data); }
    inline void clear() noexcept {
        if (!isEmpty())
            delete root;
        root = nullptr;
        _size = 0;
    }
    inline std::size_t size() const noexcept { return _size; }
#ifdef IMAGESTEGO_DEBUG
    void printDfs() const noexcept { // for debugging purposes
        if (!isEmpty())
            printDfsImpl(root);
    }
#endif
    const_iterator begin() noexcept { return Iterator(this, root, false); }
    const_iterator end() noexcept { return Iterator(this, root, true); }

private:
    class TreeNode {
        friend class AvlTree<T, Comp>;

    private:
        T _data = T();
        int _height = 1;
        TreeNode* leftChild = nullptr;
        TreeNode* rightChild = nullptr;
        TreeNode* parent = nullptr;
        explicit TreeNode(const T& data) noexcept : _data(data) {}

    public:
        virtual ~TreeNode() noexcept {
            if (leftChild)
                delete leftChild;
            if (rightChild)
                delete rightChild;
        }
        static inline int height(TreeNode* node) { return (node) ? node->_height : 0; }
        inline const T& data() const noexcept { return _data; }
        inline int balanceFactor() { return height(rightChild) - height(leftChild); }
        inline int height() {
            _height = std::max(height(rightChild), height(leftChild)) + 1;
            return _height;
        }
    }; // class TreeNode
    static TreeNode* rotateRight(TreeNode* node) {
        TreeNode* tmp = node->leftChild;
        TreeNode* T2 = tmp->rightChild;
        node->leftChild = tmp->rightChild;
        tmp->rightChild = node;
        if (T2)
            T2->parent = node;
        tmp->parent = node->parent;
        node->parent = tmp;
        node->height();
        tmp->height();
        return tmp;
    }
    static TreeNode* rotateLeft(TreeNode* node) {
        TreeNode* tmp = node->rightChild;
        TreeNode* T2 = tmp->leftChild;
        node->rightChild = tmp->leftChild;
        tmp->leftChild = node;
        if (T2)
            T2->parent = node;
        tmp->parent = node->parent;
        node->parent = tmp;
        node->height();
        tmp->height();
        return tmp;
    }
    static TreeNode* balance(TreeNode* node) {
        node->height();
        if (node->balanceFactor() == 2) {
            if (node->rightChild->balanceFactor() < 0)
                node->rightChild = rotateRight(node->rightChild);
            return rotateLeft(node);
        }
        if (node->balanceFactor() == -2) {
            if (node->leftChild->balanceFactor() > 0)
                node->leftChild = rotateLeft(node->leftChild);
            return rotateRight(node);
        }
        return node;
    }
    TreeNode* insertImpl(TreeNode* node, const T& data) {
        if (!node) {
            ++_size;
            return new TreeNode(data);
        }
        if (cmp(data, node->data())) {
            node->leftChild = insertImpl(node->leftChild, data);
            node->leftChild->parent = node;
        } else if (data != node->data()) {
            node->rightChild = insertImpl(node->rightChild, data);
            node->rightChild->parent = node;
        }
        return balance(node);
    }
    bool searchImpl(const TreeNode* node, const T& data) const {
        if (!node)
            return false;
        if (data == node->data())
            return true;
        if (cmp(data, node->data()))
            return searchImpl(node->leftChild, data);
        else
            return searchImpl(node->rightChild, data);
    }
#ifdef IMAGESTEG0_DEBUG
    static void printDfsImpl(const TreeNode* node) {
        std::cout << node->data() << std::endl;
        if (node->leftChild)
            printDfsImpl(node->leftChild);
        if (node->rightChild)
            printDfsImpl(node->rightChild);
    }
#endif
    static void copy(TreeNode* from, TreeNode* to) {
        if (from) {
            to = new TreeNode(from->data());
            copy(from->leftChild, to->leftChild);
            copy(from->rightChild, to->rightChild);
        } else {
            to = nullptr;
        }
    }
    // Base class representing tree iterator
    // Operator ++ stands for going to the next
    // node in DFS algorithm
    class Iterator { // ForwardIterator
        friend class AvlTree<T, Comp>;

    private:
        TreeNode* node;
        AvlTree<T, Comp>* owner;
        bool isEnd = false;
        explicit Iterator(AvlTree<T, Comp>* owner, TreeNode* node, bool flag) noexcept
            : node(node), owner(owner), isEnd(flag) {}

    public:
        typedef T value_type;
        inline T operator*() { return node->data(); }
        inline const T* operator->() { return &node->data(); }
        Iterator& operator++() {
            if (node->leftChild) {
                node = node->leftChild;
                return *this;
            }
            if (node->rightChild) {
                node = node->rightChild;
                return *this;
            }
            if (node != owner->root) {
                TreeNode* cameFrom;
                do {
                    cameFrom = node;
                    node = node->parent;
                } while (node->rightChild == cameFrom && node != owner->root);
                if (cameFrom == node->leftChild) {
                    if (node->rightChild) {
                        node = node->rightChild;
                        return *this;
                    } else if (node->parent) {
                        do {
                            cameFrom = node;
                            node = node->parent;
                        } while (node->rightChild == cameFrom && node != owner->root);
                        if (node != owner->root || cameFrom != node->rightChild) {
                            node = node->rightChild;
                            return *this;
                        }
                    }
                }
            }
            isEnd = true;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(*this);
            operator++();
            return tmp;
        }
        bool operator==(const Iterator& other) const noexcept {
            return node == other.node && isEnd == other.isEnd;
        }
        bool operator!=(const Iterator& other) const noexcept {
            return !(*this == other);
        }
    }; // struct Iterator
    TreeNode* root = nullptr;
    std::size_t _size = 0; // for O(1) size()
    Comp cmp = Comp();
}; // class AvlTree

template<class T1, class T2, class Pair = std::pair<T1, T2>>
struct PairComparator final {
    constexpr bool operator()(const Pair& lhs, const Pair& rhs) const noexcept {
        return lhs.first < rhs.first ||
               (lhs.first == rhs.first && lhs.second < rhs.second);
    }
};

} // namespace imagestego

#endif /* __IMAGESTEGO_CORE_AVL_TREE_HPP_INCLUDED__ */
