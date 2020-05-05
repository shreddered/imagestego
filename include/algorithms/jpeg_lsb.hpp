#ifndef __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__
#define __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__

// imagestego headers
#include "core.hpp"
#include "utils/bitarray.hpp"
#include "utils/jpeg_processor.hpp"
// c++
#include <string>
#include <vector>


namespace imagestego {

class IMAGESTEGO_EXPORTS JpegLsbStegoInserter : public AbstractStegoInserter, protected JpegProcessor {
public:
    explicit JpegLsbStegoInserter(const std::string& input, const std::string& _output); 
    void setImage(const std::string&) override;
    void setOutputName(const std::string& str) override;
    void setMessage(const std::string& message) override;
    void createStegoContainer() const override; 
protected:
    void process() const override;
private:
    mutable BitArray<> msg, key;
    std::string output;
}; // class JpegLsbStegoInserter

class IMAGESTEGO_EXPORTS JpegLsbStegoExtracter : public AbstractStegoExtracter, protected JpegProcessor {
private:
    BitArray<> key;
public:
    explicit JpegLsbStegoExtracter(const std::string& image);
    void setImage(const std::string& str);
    std::string extractMessage();
protected:
    void process() const override;
}; // class JpegLsbStegoExtracter

} // namespace imagestego

#endif /* __IMAGESTEGO_JPEG_LSB_HPP_INCLUDED__ */
