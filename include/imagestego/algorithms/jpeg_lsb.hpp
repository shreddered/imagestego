#ifndef __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
#include "imagestego/utils/jpeg_processor.hpp"
#ifdef IMAGESTEGO_ENABLE_KEYGEN_SUPPORT
#   include "imagestego/keygen.hpp"
#endif
#ifdef IMAGESTEGO_ENABLE_FORMAT_CHECKNG
#   include "imagestego/utils/format_checker.hpp"
#endif
// c++
#include <string>
#include <vector>


namespace imagestego {

class IMAGESTEGO_EXPORTS JpegLsbEmbedder : public AbstractStegoEmbedder, private JpegProcessor {
public:
    explicit JpegLsbEmbedder(const std::string& input, const std::string& _output); 
    void setImage(const std::string&) override;
    void setOutputName(const std::string& str) override;
    void setMessage(const std::string& message) override;
    void createStegoContainer() const override; 
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
private:
    mutable BitArray<> msg, key;
    std::string output;
    void process() const override;
}; // class JpegLsbStegoEmbedder

class IMAGESTEGO_EXPORTS JpegLsbExtracter : public AbstractStegoExtracter, private JpegProcessor {
public:
    explicit JpegLsbExtracter(const std::string& image);
    void setImage(const std::string& str) override;
    void setSecretKey(const std::string& key) override;
    Algorithm getAlgorithm() const noexcept override;
    std::string extractMessage() override;
private:
    BitArray<> key;
    void process() const override;
}; // class JpegLsbStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__ */
