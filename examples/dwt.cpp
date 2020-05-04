#include <algorithms/dwt.hpp>

#include <iostream>


int main() {
    imagestego::DwtStegoInserter ins("in.jpg", "out.png");
    ins.setMessage("foo");
    ins.createStegoContainer();
    imagestego::DwtStegoExtracter ext("out.png");
    std::cout << ext.extractMessage() << std::endl;
    return 0;
}
