#include <encoder.hpp>


HuffmanEncoder::HuffmanEncoder() noexcept {}

HuffmanEncoder::HuffmanEncoder(const std::string& str) noexcept : msg(str) {}

void HuffmanEncoder::setMessage(const std::string& str) noexcept {
    msg = str;
    encodedMsg.clear();
}

BitArray<unsigned char> HuffmanEncoder::getEncodedMessage() const {
    auto tree = BitArray<unsigned char>(getHuffmanTree(), 0);
    encode();
    return tree + BitArray<unsigned char>(alphabet) + BitArray<unsigned char>(encodedMsg, 0);
}

void HuffmanEncoder::encode() const {
    if (codeTable.empty())
        __buildCode();
    if (encodedMsg.empty()) {
        for (std::size_t i = 0; i != msg.length(); ++i) {
            encodedMsg += codeTable[msg[i]];
        }
    }
}

void HuffmanEncoder::__buildCode() const {
    std::map<char, std::size_t> weight;
    std::for_each(msg.begin(), msg.end(), [&weight](const char& c) mutable {
        ++weight[c];
    });
    std::multimap<std::size_t, TreeNode*> node;
    std::for_each(weight.begin(), weight.end(), [&node](const typename decltype(weight)::value_type& p) mutable {
        node.emplace(p.second, new TreeNode(p.first + std::string("")));
    }); 
    while(node.size() > 1) {
        auto it2 = node.begin(),
             it1 = it2++;
        std::for_each(it1->second->data.begin(), it1->second->data.end(), [this](const char& c) {
            codeTable[c].insert(0, 1, '0');
        });
        std::for_each(it2->second->data.begin(), it2->second->data.end(), [this](const char& c) {
            codeTable[c].insert(0, 1, '1');
        });
        typename decltype(node)::value_type pair = {it1->first + it2->first,
            new TreeNode(it1->second->data + it2->second->data)};
        pair.second->right = it2->second;
        pair.second->left = it1->second;
        node.erase(it1);
        node.erase(it2);
        node.insert(pair);
    }
    root = node.begin()->second;
}

std::string HuffmanEncoder::getHuffmanTree() const {
    if (codeTable.empty()) {
        __buildCode();
    }
    route.clear();
    alphabet.clear();
    // creating DFS string
    dfs(root);
    route += '0';
    return route;
}

void HuffmanEncoder::dfs(HuffmanEncoder::TreeNode* node) const {
    if (node->left) {
        route += '1';
        dfs(node->left);
        route += '0';
        dfs(node->right);
        if (route.back() != '0')
            route += '0';
    }
    else {
        alphabet += node->data;
    }
}

HuffmanEncoder::~HuffmanEncoder() noexcept {
    if (root)
        destroyNode(root);
}

void HuffmanEncoder::destroyNode(HuffmanEncoder::TreeNode* node) noexcept {
    if (node->left) {
        destroyNode(node->left);
        destroyNode(node->right);
        delete node;
    }
}
