#include "imagestego/utils/jpeg_processor.hpp"


namespace imagestego {

JpegProcessor::JpegProcessor(const std::string& src) : input(fopen(src.c_str(), "rb")) {
//    compressInfo.err = jpeg_std_error(&err);
    decompressInfo.err = jpeg_std_error(&err);
    jpeg_create_decompress(&decompressInfo);
//    jpeg_create_compress(&compressInfo);
    jpeg_stdio_src(&decompressInfo, input); 
//    jpeg_stdio_dest(&compressInfo, output);
    jpeg_read_header(&decompressInfo, static_cast<boolean>(1));
//    compressInfo.dct_method = JDCT_ISLOW;
    dctCoeffs = jpeg_read_coefficients(&decompressInfo);
}

JpegProcessor::~JpegProcessor() noexcept {
    jpeg_finish_decompress(&decompressInfo);
    fclose(input);
    jpeg_destroy_decompress(&decompressInfo);
}

JCOEFPTR JpegProcessor::getBlock(const int& channel, const int& y, const int& x) const {
    auto buf = (decompressInfo.mem->access_virt_barray)(reinterpret_cast<j_common_ptr>(&decompressInfo), dctCoeffs[channel], 
            y, 1, static_cast<boolean>(1));
    return buf[0][x];
}

std::pair<int, int> JpegProcessor::getChannelSize(const int& channel) const {
    auto compInfo = decompressInfo.comp_info + channel;
    return std::make_pair(compInfo->height_in_blocks, compInfo->width_in_blocks);
}

void JpegProcessor::writeTo(const std::string& dst) const {
    FILE* fd = fopen(dst.c_str(), "wb");
    jpeg_compress_struct compressInfo;
    jpeg_error_mgr jerr;
    compressInfo.err = jpeg_std_error(&jerr); 
    jpeg_create_compress(&compressInfo);
    jpeg_stdio_dest(&compressInfo, fd);
    compressInfo.dct_method = JDCT_ISLOW;
    jpeg_copy_critical_parameters(&decompressInfo, &compressInfo);
    compressInfo.input_components = 3;
    compressInfo.in_color_space = JCS_RGB;
    jpeg_write_coefficients(&compressInfo, dctCoeffs);
    jpeg_finish_compress(&compressInfo);
    fclose(fd);
    jpeg_destroy_compress(&compressInfo);
}

} // namespace imagestego
