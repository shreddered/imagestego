#include "imagestego/imagestego.hpp"


void imagestego::embedSecretMessage(AbstractStegoEmbedder* embedder, const std::string& image,
        const std::string& output, const std::string& message, const std::string& key) {
    std::unique_ptr<AbstractStegoEmbedder> ptr(embedder);
    embedder->setImage(image);
    embedder->setOutputName(output);
    embedder->setMessage(message);
    embedder->setSecretKey(key);
    embedder->createStegoContainer();
}

std::string imagestego::extractSecretMessage(AbstractStegoExtracter* extracter, const std::string& image, const std::string& key) {
    std::unique_ptr<AbstractStegoExtracter> ptr(extracter);
    extracter->setImage(image);
    extracter->setSecretKey(key);
    return extracter->extractMessage();
}
