#ifndef __IMAGESTEGO_F5_HPP_INCLUDED__
#define __IMAGESTEGO_F5_HPP_INCLUDED__

// imagestego headers
#include "imagestego/core.hpp"
#include "imagestego/utils/bitarray.hpp"
// c++ headers
#include <string>


namespace imagestego {

class F5Embedder : public AbstractStegoEmbedder {
public:
    explicit F5Embedder() noexcept;
    explicit F5Embedder(const std::string& imagename, const std::string& output);
    void setImage(const std::string& image) override;
    void setOutputName(const std::string& dst) override;
    void setMessage(const std::string& msg) override;
    void setSecretKey(const std::string& key) override;
    void createStegoContainer() const override;
    Algorithm getAlgorithm() const override;
private:
    std::string outputFile;
    BitArray<> msg;
}; // class F5Embedder

}; //namespace imagestego

#endif /* __IMAGESTEGO_F5_HPP_INCLUDED__ */
