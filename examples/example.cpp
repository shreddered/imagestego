#include <imagestego.hpp>
#include <utils/bitarray.hpp>
#include <algorithms/lsb_stego.hpp>

#include <bitset>
#include <iostream>


int main(int argc, char** agrv) {
    //std::cout << "Hello, world!" << a <<std::endl;
    /*Lsb lsb;
    lsb.setMessage("Hello!");
    lsb.setImage("nice_man.jpg");
    lsb.setOutputName("nice_stego.png");
    lsb.createStegoContainer();*/
    Lsb lsb;
    lsb.setImage("nice_stego.png");
    std::cout << lsb.extractMessage() << std::endl;
    return 0;
}
