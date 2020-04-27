#ifndef __IMAGESTEGO_BINTREE_HPP_INCLUDED__
#define __IMAGESTEGO_BINTREE_HPP_INCLUDED__

#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>


template<typename T, class Comp = std::less<T>>
class BinaryTree {
protected:
    class TreeNode {
        friend class BinaryTree<T, Comp>;
    private:
        T _data = T();
        int _height = 1;
        TreeNode* leftChild = nullptr;
        TreeNode* rightChild = nullptr;
        TreeNode* parent = nullptr;
        explicit TreeNode(const T& data) noexcept : parent(parent), _data(data) {}
    public:
        static inline int height(TreeNode* node) {
            return (node) ? node->_height : 0;
        }
        inline const T& data() const noexcept {
            return _data;
        }
        inline T& data() noexcept {
            return _data;
        }
        inline int balanceFactor() {
            return height(rightChild) - height(leftChild);
        }
        inline int height() {
            _height = std::max(height(rightChild), height(leftChild)) + 1;
            return _height;
        }
    }; // class TreeNode
    static TreeNode* rotateRight(TreeNode* node) {
        TreeNode* tmp = node->leftChild;
        TreeNode* T2 = tmp->rightChild;
        tmp->rightChild = node;
        node->leftChild = T2;
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
        tmp->leftChild = node;
        node->rightChild = T2;

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
    static TreeNode* insertImpl(TreeNode* node, const T& data) {
        if (!node)
            return new TreeNode(data);
        if (data < node->data()) {
            node->leftChild = insertImpl(node->leftChild, data);
            node->leftChild->parent = node;
        }
        else {
            node->rightChild = insertImpl(node->rightChild, data);
            node->rightChild->parent = node;
        }
        return balance(node);
    }
    static void printDfsImpl(TreeNode* node) {
        std::cout << node->data() << std::endl;
        if (node->leftChild)
            printDfsImpl(node->leftChild);
        if (node->rightChild)
            printDfsImpl(node->rightChild);
    }
    // Base class representing tree iterator
    // Both operators ++ and -- stands for going to the next or 
    // to the previous node if DFS respectively
    class Iterator { // ForwardIterator
        friend class BinaryTree<T, Comp>;
    private:
        TreeNode* node;
        BinaryTree<T, Comp>* owner;
        bool isEnd = false;
        explicit Iterator(BinaryTree<T, Comp>* owner, TreeNode* node, bool flag) noexcept : owner(owner), node(node), isEnd(flag) {}
    public:
        inline T operator *() {
            return node->data();
        }
        Iterator& operator ++() {
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
                    }
                    else {
                        node = node->parent->rightChild;
                        return *this;
                    }
                }
            }
            isEnd = true;
            return *this;
        }
        Iterator operator ++(int) {
            Iterator tmp(*this);
            operator ++();
            return tmp;
        }
        bool operator ==(const Iterator& other) const noexcept {
            return node == other.node && isEnd == other.isEnd;
        }
        bool operator !=(const Iterator& other) const noexcept {
            return !(*this == other);
        }
    }; // struct Iterator
    TreeNode* root = nullptr;
public:
    explicit BinaryTree() noexcept {}
    template<class It>
    explicit BinaryTree(It begin, It end) {
        for (auto it = begin; it != end; ++it)
            insert(*it);
    }
    void insert(const T& data) noexcept {
        root = insertImpl(root, data);
    }
    inline bool isEmpty() const noexcept {
        return root == nullptr;
    }
    void printDfs() noexcept { // for debugging purposes
        if (!isEmpty())
            printDfsImpl(root);
    }
    Iterator begin() noexcept {
        return Iterator(this, root, false);
    }
    Iterator end() noexcept {
        return Iterator(this, root, true);
    }
}; // class BinaryTree

#endif /* __IMAGESTEGO_BINTREE_HPP_INCLUDED__ */
