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
#include "imagestego/compression/huffman_encoder.hpp"
// c++ headers
#include <string>
#include <unordered_map>


namespace imagestego {

class HuffmanEncoderImpl final {
public:
    explicit HuffmanEncoderImpl() noexcept {}
    explicit HuffmanEncoderImpl(const std::string& str) noexcept : msg(str) {}
    HuffmanEncoderImpl(const HuffmanEncoderImpl&) = delete;
    HuffmanEncoderImpl& operator =(const HuffmanEncoderImpl&) = delete;
    void setMessage(const std::string& str) noexcept {
        msg = str;
        encodedMsg.clear();
    }
    BitArray getEncodedMessage() {
        encode();
        return BitArray(encodedMsg);
    }
    void getHuffmanTree() {
        if (codeTable.empty()) {
            __buildCode();
        }
        route.clear();
        alphabet.clear();
        // creating DFS string
        dfs(root);
        route += '0';
    }
    std::string getAlphabet() const noexcept {
        return alphabet;
    }
    virtual ~HuffmanEncoderImpl() noexcept {
        if (root)
            delete root;
    }
private:
    struct TreeNode final {
        std::string data;
        bool isVisited = false;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        TreeNode(const std::string& data) noexcept : data(data) {}
        ~TreeNode() noexcept {
            if (left)
                delete left;
            if (right)
                delete right;
        }
    };
    void __buildCode() {
        std::unordered_map<char, std::size_t> weight;
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
    void encode() {
        if (codeTable.empty())
            __buildCode();
        if (encodedMsg.empty()) {
            getHuffmanTree();
            encodedMsg.insert(0, BitArray::fromByteString(alphabet).toString());
            encodedMsg.insert(0, route);
            for (std::size_t i = 0; i != msg.length(); ++i) {
                encodedMsg += codeTable[msg[i]];
            }
        }
    }
    void dfs(TreeNode* node) {
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
    std::string msg;
    TreeNode* root = nullptr;
    std::string encodedMsg = ""; // mutable bc of caching
    std::unordered_map<char, std::string> codeTable;
    std::string route = "";
    std::string alphabet;
}; // class HuffmanEncoderImpl

HuffmanEncoder::HuffmanEncoder() noexcept : encoder(new HuffmanEncoderImpl()) {}

HuffmanEncoder::HuffmanEncoder(const std::string& str) noexcept : encoder(new HuffmanEncoderImpl(str)) {}

HuffmanEncoder::HuffmanEncoder(std::string&& str) noexcept : encoder(new HuffmanEncoderImpl(str)) {}

void HuffmanEncoder::setMessage(const std::string& str) {
    encoder->setMessage(str);
}

BitArray HuffmanEncoder::getEncodedMessage() {
    return encoder->getEncodedMessage();
}

HuffmanEncoder::~HuffmanEncoder() noexcept {
    delete encoder;
}

} // namespace imagestego
