// imagestego headers
#include "imagestego/imagestego.hpp"
// c++ headers
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>

#define USAGE_MSG "Usage: imagestego [mode] [options]\n" \
"Mode:\n" \
"\tembed - perform embedding\n" \
"\textract - perform extraction\n" \
"Options:\n" \
"\t-h                print this help message and exit\n" \
"\t-i [file]         image in which message will be embedded or image from which\n" \
"\t                  message should be extracted\n" \
"\t-o [file]         name of stego container in which embedding should be performed\n" \
"\t-m [message]      message to embed in image\n" \
"\t-M [file]         file with message to embed in message\n" \
"\t-k [key]          specify secret key\n" \
"\t-K [file]         specify file with secret key\n" \
"\t-a [algorithm]    embedding/extraction algorithm\n" \
"\tAlgorithm:\n" \
"\t\tdwt\n" \
"\t\tlsb\n" \
"\t\tjpeg_lsb\n" \
"\t-c [algorithm]    (de)compress input\n"\
"\tAlgorithm:\n" \
"\t\tlzw\n" \
"\t\thuffman"


std::map<std::string, std::string> getopts(int argc, char** argv);

void check(const std::map<std::string, std::string>& opts);

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << USAGE_MSG << std::endl;
        return 1;
    }
    std::string mode = std::string(argv[1]);
    if (argc == 2) {
        if (mode == "-h") {
            std::cout << USAGE_MSG << std::endl;
            return 0;
        }
    }
    if (mode == "embed") {
        try {
            auto opts = getopts(argc - 2, argv + 2);
            const auto compress = opts.find("-c"),
                       algo = opts.find("-a"),
                       keyFile = opts.find("-K"),
                       input = opts.find("-i"),
                       output = opts.find("-o");
            auto msg = opts.find("-M");
            if (algo == opts.end())
                throw std::invalid_argument("Error: no algoritm was specified");
            if (input == opts.end())
                throw std::invalid_argument("Error: no input file");
            if (output == opts.end())
                throw std::invalid_argument("Error: no output file");
            std::string message;
            if (msg == opts.end()) {
                msg = opts.find("-m");
                if (msg == opts.end())
                    throw std::invalid_argument("Error: no message");
                else
                    message = msg->second;
            }
            else {
                std::ifstream f(msg->second, std::ios_base::binary);
                message = std::string((std::istreambuf_iterator<char>(f)),
                                      std::istreambuf_iterator<char>());
            }
            std::string key;
            if (keyFile != opts.end()) { // key file preferred
                std::ifstream f(keyFile->second, std::ios_base::binary);
                key = std::string((std::istreambuf_iterator<char>(f)), 
                                  std::istreambuf_iterator<char>());
                f.close();
            }
            else if(opts.find("-k") != opts.end())
                key = opts["-k"];
            else
                throw std::invalid_argument("Error: no secret key");
            const std::string embAlgo = algo->second;
            if (compress != opts.end()) {
                const std::string compAlgo = compress->second;
                if (compAlgo == "lzw") {
                    if (embAlgo == "lsb") {
                        imagestego::embedSecretMessage(new imagestego::LsbEmbedder<imagestego::LzwEncoder>, 
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else if (embAlgo == "dwt") {
                        imagestego::embedSecretMessage(new imagestego::DwtEmbedder<imagestego::LzwEncoder>,
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else if (embAlgo == "jpeg_lsb") {
                        imagestego::embedSecretMessage(new imagestego::JpegLsbEmbedder<imagestego::LzwEncoder>,
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else
                        throw std::invalid_argument("Error: unknown algorithm '" + embAlgo + '\'');
                } // if lzw
                else if (compAlgo == "huffman") {
                    if (embAlgo == "lsb") {
                        imagestego::embedSecretMessage(new imagestego::LsbEmbedder<imagestego::HuffmanEncoder>, 
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else if (embAlgo == "dwt") {
                        imagestego::embedSecretMessage(new imagestego::DwtEmbedder<imagestego::HuffmanEncoder>,
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else if (embAlgo == "jpeg_lsb") {
                        imagestego::embedSecretMessage(new imagestego::JpegLsbEmbedder<imagestego::HuffmanEncoder>,
                                                       input->second, output->second, message, key);
                        return 0;
                    }
                    else
                        throw std::invalid_argument("Error: unknown stego algorithm '" + embAlgo + '\'');
                } // if huffman
            } // if compression
            if (embAlgo == "lsb") {
                imagestego::embedSecretMessage(new imagestego::LsbEmbedder<imagestego::LzwEncoder>, 
                                               input->second, output->second, message, key);
                return 0;
            }
            else if (embAlgo == "dwt") {
                imagestego::embedSecretMessage(new imagestego::DwtEmbedder<imagestego::LzwEncoder>,
                                               input->second, output->second, message, key);
                return 0;
            }
            else if (embAlgo == "jpeg_lsb") {
                imagestego::embedSecretMessage(new imagestego::JpegLsbEmbedder<imagestego::LzwEncoder>,
                                               input->second, output->second, message, key);
                return 0;
            }
            else
                throw std::invalid_argument("Error: unknown algorithm '" + embAlgo + '\''); 
        } // try
        catch(const std::invalid_argument& e) {
            std::cerr << USAGE_MSG << std::endl;
            std::cerr << e.what() << std::endl;
            return 1;
        }
        catch(const imagestego::Exception& e) {
            std::cerr << e.what() << std::endl;
            return 2;
        }
    } // if mode == embed 
    else if (mode == "extract") {
        try {
            auto opts = getopts(argc - 2, argv + 2);
            const auto compress = opts.find("-c"),
                       algo = opts.find("-a"),
                       keyFile = opts.find("-K"),
                       input = opts.find("-i"),
                       output = opts.find("-o");
            if (algo == opts.end())
                throw std::invalid_argument("Error: no algoritm was specified");
            if (input == opts.end())
                throw std::invalid_argument("Error: no input file");
            std::string key, message;
            if (keyFile != opts.end()) { // key file preferred
                std::ifstream f(keyFile->second, std::ios_base::binary);
                key = std::string((std::istreambuf_iterator<char>(f)),
                                  std::istreambuf_iterator<char>());
                f.close();
            }
            else if(opts.find("-k") != opts.end())
                key = opts["-k"];
            else
                throw std::invalid_argument("Error: no secret key");
            std::string extAlgo = algo->second;
            if (compress != opts.end()) {
                const std::string compAlgo = compress->second;
                if (compAlgo == "lzw") {                    
                    if (extAlgo == "dwt") {
                        message = imagestego::extractSecretMessage(new imagestego::DwtExtracter<imagestego::LzwDecoder>,
                                                                   input->second, key);
                    }
                    else if (extAlgo == "lsb") {
                        message = imagestego::extractSecretMessage(new imagestego::LsbExtracter<imagestego::LzwDecoder>,
                                                                   input->second, key);
                    }
                    else if (extAlgo == "jpeg_lsb") {                
                        message = imagestego::extractSecretMessage(new imagestego::JpegLsbExtracter<imagestego::LzwDecoder>,
                                                                   input->second, key);
                    }
                } // if lzw
                else if (compAlgo == "huffman") {
                    if (extAlgo == "dwt") {
                        message = imagestego::extractSecretMessage(new imagestego::DwtExtracter<imagestego::HuffmanDecoder>,
                                                                   input->second, key);
                    }
                    else if (extAlgo == "lsb") {
                        message = imagestego::extractSecretMessage(new imagestego::LsbExtracter<imagestego::HuffmanDecoder>,
                                                                   input->second, key);
                    }
                    else if (extAlgo == "jpeg_lsb") {                
                        message = imagestego::extractSecretMessage(new imagestego::JpegLsbExtracter<imagestego::HuffmanDecoder>,
                                                                   input->second, key);
                    }
                } // elif huffman
            } // if compress
            if (message.empty()) {
                if (extAlgo == "dwt") {
                    message = imagestego::extractSecretMessage(new imagestego::DwtExtracter<void>,
                                                               input->second, key);
                }
                else if (extAlgo == "lsb") {
                    message = imagestego::extractSecretMessage(new imagestego::LsbExtracter<void>,
                                                               input->second, key);
                }
                else if (extAlgo == "jpeg_lsb") {                
                    message = imagestego::extractSecretMessage(new imagestego::JpegLsbExtracter<void>,
                                                               input->second, key);
                }
            }
            if (output == opts.end()) {
                std::cout << message << std::endl;
                return 0;
            }
            std::ofstream f(output->second, std::ios_base::binary);
            f.write(message.data(), message.length());
            return 0;
        } // try
        catch(const std::invalid_argument& e) {
            std::cerr << USAGE_MSG << std::endl;
            std::cerr << e.what() << std::endl;
            return 2;
        }
    } // mode == extract
    else {
        std::cerr << USAGE_MSG << std::endl;
        std::cerr << "Error: unknown mode '" << mode << '\'' << std::endl;
        return 2;
    }
}

std::map<std::string, std::string> getopts(int argc, char** argv) {
    if (argc % 2 != 0 || argc == 0)
        throw std::invalid_argument("Error: wrong number of arguments");
    std::map<std::string, std::string> options;
    for (int i = 0; i < argc; i += 2) {
        options.emplace(argv[i], argv[i + 1]);
    }
    return options;
}
