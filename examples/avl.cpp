#include <utils/bintree.hpp>


int main() {
    auto v = {1, 3, 5, 7, 6};
    BinaryTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    tree.printDfs();
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << std::endl;
    }
    return 0;
}
