#include <utils/binary_tree.hpp>


int main() {
    BinaryTree<int> tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.printDfs();
    return 0;
}