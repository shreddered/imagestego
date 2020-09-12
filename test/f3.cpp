#include "imagestego/algorithms/f3.hpp"
#include "imagestego/compression.hpp"

using namespace imagestego;


int main() {
    F3Embedder<HuffmanEncoder> emb("in.jpg", "out.jpg");
    emb.setMessage("message");
    emb.createStegoContainer();
    return 0;
}
