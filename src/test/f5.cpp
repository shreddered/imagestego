#include "imagestego/utils/jpeg_image.hpp"

using imagestego::JpegImage;


int main() {
    JpegImage image("in.jpg");
    image.writeTo("out.jpg");
    return 0;
}
