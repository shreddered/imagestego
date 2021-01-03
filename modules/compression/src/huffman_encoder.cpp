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
    explicit HuffmanEncoderImpl(const std::string& str) noexcept : _msg(str) {}
    HuffmanEncoderImpl(const HuffmanEncoderImpl&) = delete;
    HuffmanEncoderImpl& operator=(const HuffmanEncoderImpl&) = delete;
    void setMessage(const std::string& str) noexcept {
        _msg = str;
        _encodedMsg.clear();
    }
    BitArray getEncodedMessage() {
        encode();
        return BitArray(_encodedMsg);
    }
    void getHuffmanTree() {
        if (_codeTable.empty()) {
            __buildCode();
        }
        _route.clear();
        _alphabet.clear();
        // creating DFS string
        dfs(_root);
        _route += '0';
    }
    std::string getAlphabet() const noexcept { return _alphabet; }
    virtual ~HuffmanEncoderImpl() noexcept {
        if (_root)
            delete _root;
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
        std::for_each(_msg.begin(), _msg.end(),
                      [&weight](const char& c) mutable { ++weight[c]; });
        std::multimap<std::size_t, TreeNode*> node;
        std::for_each(weight.begin(), weight.end(),
                      [&node](const typename decltype(weight)::value_type& p) mutable {
                          node.emplace(p.second, new TreeNode(p.first + std::string("")));
                      });
        while (node.size() > 1) {
            auto it2 = node.begin(), it1 = it2++;
            std::for_each(it1->second->data.begin(), it1->second->data.end(),
                          [this](const char& c) { _codeTable[c].insert(0, 1, '0'); });
            std::for_each(it2->second->data.begin(), it2->second->data.end(),
                          [this](const char& c) { _codeTable[c].insert(0, 1, '1'); });
            typename decltype(node)::value_type pair = {
                it1->first + it2->first,
                new TreeNode(it1->second->data + it2->second->data)};
            pair.second->right = it2->second;
            pair.second->left = it1->second;
            node.erase(it1);
            node.erase(it2);
            node.insert(pair);
        }
        _root = node.begin()->second;
    }
    void encode() {
        if (_codeTable.empty())
            __buildCode();
        if (_encodedMsg.empty()) {
            getHuffmanTree();
            _encodedMsg.insert(0, BitArray::fromByteString(_alphabet).toString());
            _encodedMsg.insert(0, _route);
            for (std::size_t i = 0; i != _msg.length(); ++i) {
                _encodedMsg += _codeTable[_msg[i]];
            }
        }
    }
    void dfs(TreeNode* node) {
        if (node->left) {
            _route += '1';
            dfs(node->left);
            _route += '0';
            dfs(node->right);
            if (_route.back() != '0')
                _route += '0';
        } else {
            _alphabet += node->data;
        }
    }
    std::string _msg;
    TreeNode* _root = nullptr;
    std::string _encodedMsg = "";
    std::unordered_map<char, std::string> _codeTable;
    std::string _route = "";
    std::string _alphabet;
}; // class HuffmanEncoderImpl

HuffmanEncoder::HuffmanEncoder() noexcept : encoder(new HuffmanEncoderImpl()) {}

HuffmanEncoder::HuffmanEncoder(const std::string& str) noexcept
    : encoder(new HuffmanEncoderImpl(str)) {}

HuffmanEncoder::HuffmanEncoder(std::string&& str) noexcept
    : encoder(new HuffmanEncoderImpl(str)) {}

void HuffmanEncoder::setMessage(const std::string& str) { encoder->setMessage(str); }

BitArray HuffmanEncoder::getEncodedMessage() { return encoder->getEncodedMessage(); }

HuffmanEncoder::~HuffmanEncoder() noexcept { delete encoder; }

} // namespace imagestego
