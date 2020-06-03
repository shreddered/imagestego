#include "imagestego/utils/jpeg_image.hpp"
#include <iostream>


namespace imagestego {

Point::Point(short& _x, short& _y, short& _z) noexcept : x(_x), y(_y), z(_z) {}

short& Point::operator [](int index) {
    if (!index)
        return x;
    else if (index - 1)
        return z;
    else
        return y;
}

JpegImage::JpegImage() noexcept {}

JpegImage::JpegImage(const std::string& src) {
    open(src);
}

JpegImage::~JpegImage() noexcept {
    close();
}

void JpegImage::open(const std::string& src) {
    if (in)
        close();
    in = fopen(src.c_str(), "rb");
    if (!in)
        throw Exception(Exception::Codes::NoSuchFile);
    dinfo.err = jpeg_std_error(&err);
    jpeg_create_decompress(&dinfo);
    jpeg_stdio_src(&dinfo, in);
    jpeg_read_header(&dinfo, static_cast<boolean>(true));
    coeffs = jpeg_read_coefficients(&dinfo);
    rows = dinfo.image_height & ~7;
    cols = dinfo.image_width & ~7;
}

void JpegImage::close() noexcept {
    jpeg_finish_decompress(&dinfo);
    if (in)
        fclose(in);
    in = nullptr;
    jpeg_destroy_decompress(&dinfo);
}

Point JpegImage::at(const int& y, const int& x) {
    auto red = (dinfo.mem->access_virt_barray)(reinterpret_cast<j_common_ptr>(&dinfo), coeffs[0],
                y / 8, 1, static_cast<boolean>(true)),
         green = (dinfo.mem->access_virt_barray)(reinterpret_cast<j_common_ptr>(&dinfo), coeffs[1],
                y / 8, 1, static_cast<boolean>(true)),
         blue = (dinfo.mem->access_virt_barray)(reinterpret_cast<j_common_ptr>(&dinfo), coeffs[2],
                y / 8, 1, static_cast<boolean>(true));
    return Point(red[0][x / 8][(y % 8) * 8 + (x % 8)],
                 green[0][x / 8][(y % 8) * 8 + (x % 8)],
                 blue[0][x / 8][(y % 8) * 8 + (x % 8)]);
}

void JpegImage::writeTo(const std::string& dst) {
    jpeg_compress_struct cinfo;
    cinfo.err = jpeg_std_error(&err);
    jpeg_create_compress(&cinfo);
    FILE* out = fopen(dst.c_str(), "wb");
    jpeg_stdio_dest(&cinfo, out);
    jpeg_copy_critical_parameters(&dinfo, &cinfo);
    cinfo.input_components = 3;
    cinfo.in_color_space = dinfo.out_color_space;
    jpeg_write_coefficients(&cinfo, coeffs);
    jpeg_finish_compress(&cinfo);
    fclose(out);
    jpeg_destroy_compress(&cinfo);
}

}
