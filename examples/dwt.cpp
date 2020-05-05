#include <imagestego/algorithms/dwt.hpp>

#include <iostream>


int main() {
    imagestego::DwtEmbedder ins("in.jpg", "out.png");
    ins.setMessage("foo");
    ins.createStegoContainer();
    imagestego::DwtExtracter ext("out.png");
    std::cout << ext.extractMessage() << std::endl;
    return 0;
}
