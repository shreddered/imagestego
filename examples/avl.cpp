#include <utils/bintree.hpp>

#include <vector>


int main() {
    srand(1);
    std::vector<int> v;
    for (int i = 0; i < 100; ++i)
        v.push_back(rand());
    BinaryTree<int> tree;
    for (auto elem : v)
        tree.insert(elem);
    tree.printDfs();
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << "size = " << tree.size() << std::endl;
    /*srand(time(nullptr));
    Route r(std::make_pair(500, 500));
    r.create(10);
    for (auto elem : r)
        std::cout << '[' << elem.first << "; " << elem.second << ']' << std::endl;
        */
    return 0;
}
