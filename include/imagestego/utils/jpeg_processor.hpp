#ifndef __IMAGESTEGO_JPEG_PROCESSOR_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_PROCESSOR_HPP_INCLUDED__

// c++
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <utility>
// libjpeg
#include <jpeglib.h>


namespace imagestego {

/*
 * TODO: reduce amount of code
 */ 
class JpegProcessor {
public:
    virtual ~JpegProcessor() noexcept;
protected:
    explicit JpegProcessor() noexcept;
    explicit JpegProcessor(const std::string& src);
    virtual void process() const = 0;
    void read(const std::string& src);
    void writeTo(const std::string& dst) const;
    JCOEFPTR getBlock(const int& channel, const int& y, const int& x) const;
    std::pair<int, int> getChannelSize(const int& channel) const;
    short& at(const int& y, const int& x, const int& channel = 0) const;
private:
    FILE* input = nullptr;
    mutable jpeg_decompress_struct decompressInfo;
    jpeg_error_mgr err;
    jvirt_barray_ptr* dctCoeffs;
}; // class JpegImage

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_PROCESSOR_HPP_INCLUDED__ */
