#include <imagestego/algorithms/dwt.hpp>

#include <iostream>


int main() {
    imagestego::DwtEmbedder<void> ins("in.jpg", "out.png");
    ins.setMessage("ass we can!");
    ins.setSecretKey("pwd");
    ins.createStegoContainer();
    imagestego::DwtExtracter<void> ext("out.png");
    ext.setSecretKey("pwd");
    std::cout << ext.extractMessage() << std::endl;
    return 0;
}
