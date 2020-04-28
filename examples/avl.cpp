#include <utils/bintree.hpp>


int main() {
    auto v = {1, 1, 3, 5, 7, 6, 3};
    BinaryTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << "size = " << tree.size() << std::endl;
    return 0;
}
