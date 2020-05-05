#ifndef __IMAGESTEGO_BINARY_TREE_HPP_INCLUDED__
#define __IMAGESTEGO_BINARY_TREE_HPP_INCLUDED__

#include <functional>
#ifdef _DEBUG
#   include <iostream>
#endif
#include <optional>
#include <utility>


namespace imagestego {

// Base class template standing for binary (non-balanced) tree implementation
//      T - data type;
//      Comp - functional object for comparison
template<typename T, class Comp = std::less<T>>
class BinaryTree {
private:
    class TreeNode final {
        friend class BinaryTree<T, std::less<T>>;
        T data;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        explicit TreeNode(const T& _data) noexcept : data(_data) {}
    public:
        ~TreeNode() noexcept {
            if (left)
                delete left;
            if (right)
                delete right;
        }
    }; // class TreeNode
    TreeNode* root = nullptr;
    std::size_t _size = 0;
    Comp cmp = Comp();
    TreeNode* insertImpl(TreeNode* node, const T& data) {
        if (!node) {
            ++_size;
            return new TreeNode(data);
        }
        if (cmp(data, node->data)) 
            node->left = insertImpl(node->left, data);
        else if (data != node->data)
            node->right = insertImpl(node->right, data);
        return node;
    }
    TreeNode* searchImpl(TreeNode* node, const T& data, bool flag) {
        if (!node)
            return (flag) ? insertImpl(node, data) : nullptr;
        if (data == node->data)
            return node;
        if (cmp(data, node->data))
            return searchImpl(node->left, data);
        else
            return searchImpl(node->right, data);
    }
#ifdef _DEBUG
    static void printDfsImpl(const TreeNode* node) {
        std::cout << node->data << std::endl;
        if (node->left)
            printDfsImpl(node->left);
        if (node->right)
            printDfsImpl(node->right);
    }
#endif
public:
    explicit BinaryTree() noexcept {}
    virtual ~BinaryTree() noexcept {
        if (root)
            delete root;
    }
    inline void clear() noexcept {
        if (root)
            delete root;
        _size = 0;
    }
    void insert(const T& data) {
        root = insertImpl(root, data);
    }
    bool search(const T& data, bool flag = false) {
        auto node = searchImpl(root, data, flag);
        return bool(node);
    }
    inline bool isEmpty() const noexcept {
        return root == nullptr;
    }
    inline std::size_t size() const noexcept {
        return _size;
    }
#ifdef _DEBUG
    void printDfs() const noexcept { // for debugging purposes
        if (!isEmpty())
            printDfsImpl(root);
    }
#endif
}; // clas BinaryTree

} // namespace imagestego

#endif /* __IMAGESTEGO_BINARY_TREE_HPP_INCLUDED__ */
