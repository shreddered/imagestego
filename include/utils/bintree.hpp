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
        explicit TreeNode(const T& data) noexcept : _data(data) {}
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
        if (Comp()(data, node->data())) {
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
    static void copy(TreeNode* from, TreeNode* to) {
        if (from) {
            to = new TreeNode(from->data());
            copy(from->leftChild, to->leftChild);
            copy(from->rightChild, to->rightChild);
        }
        else 
            to = nullptr;
    }
    static void destroy(TreeNode* node) noexcept {
        if (node->leftChild)
            destroy(node->leftChild);
        if (node->rightChild)
            destroy(node->rightChild);
    }
    // Base class representing tree iterator
    // Operator ++ stands for going to the next 
    // node in DFS algorithm
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
    std::size_t _size = 0; // for O(1) size()
public:
    explicit BinaryTree() noexcept {}
    template<class It>
    explicit BinaryTree(It begin, It end) : _size(std::distance(begin, end)) {
        for (auto it = begin; it != end; ++it)
            insert(*it);
    }
    BinaryTree(const BinaryTree<T, Comp>& other) : _size(other._size) {
        copy(other.root, root);
    }
    BinaryTree(BinaryTree<T, Comp>&& other) noexcept : root(other.root), _size(other._size) {
        other.root = nullptr;
    }
    virtual ~BinaryTree() noexcept {
        if (!isEmpty())
            destroy(root);
    }
    BinaryTree<T, Comp>& operator =(BinaryTree<T, Comp>&& other) noexcept {
        root = other.root;
        _size = other._size;
        other.root = nullptr;
        return *this;
    }
    BinaryTree<T, Comp>& operator =(const BinaryTree<T, Comp>& other) {
        if (this != &other)
            copy(other.root, root);
        return *this;
    }
    void insert(const T& data) noexcept {
        root = insertImpl(root, data);
        ++_size;
    }
    inline bool isEmpty() const noexcept {
        return root == nullptr;
    }
    inline void clear() noexcept {
        if (!isEmpty())
            destroy(root);
        root = nullptr;
    }
    inline std::size_t size() const noexcept {
        return _size;
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

template<class T1, class T2, class Pair = std::pair<T1, T2>>
struct PairComparator {
    constexpr bool operator ()(const Pair& lhs, const Pair& rhs) const noexcept {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }
};

class Route final : public BinaryTree<std::pair<int, int>, PairComparator<int, int>> {
public:
    explicit Route() noexcept : BinaryTree() {}
    template<class It>
    explicit Route(It begin, It end) : BinaryTree(begin, end) {}
    void create(const int& n) {
    }
}; // class Route

#endif /* __IMAGESTEGO_BINTREE_HPP_INCLUDED__ */
