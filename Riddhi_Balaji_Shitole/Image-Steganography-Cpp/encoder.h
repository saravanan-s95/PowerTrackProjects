#ifndef ENCODER_H
#define ENCODER_H

#include "common.h"
#include "image.h"

class Encoder
{
private:
    std::string inputImage;
    std::string secretFile;
    std::string outputImage;

    Image image;

    std::ifstream secretInput;

    long imageCapacity;
    long secretFileSize;
public:
    Encoder(const std::string &input,
            const std::string &secret,
            const std::string &output);

    bool encode();
private:
    bool openFiles();
    bool checkCapacity();
    bool encodeMagicString();

    bool encodeInt(int value);
    bool encodeString(const std::string &str);

    bool encodeFileExtension();
    bool encodeFileSize();

    bool encodeSecretFile();
};

#endif