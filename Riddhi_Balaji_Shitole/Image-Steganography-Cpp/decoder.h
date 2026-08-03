#ifndef DECODER_H
#define DECODER_H

#include "common.h"
#include "image.h"

class Decoder
{
private:
    std::string inputImage;
    std::string outputFile;

    std::ifstream imageFile;
    std::ofstream secretOutput;

    int secretFileSize;

public:
    Decoder(const std::string &input,
            const std::string &output);

    bool decode();

private:
    bool openFiles();

    char decodeByte();
    int decodeInt();
    std::string decodeString(int length);

    bool decodeMagicString();
    bool decodeExtension();
    bool decodeFileSize();
    bool decodeSecretFile();
};

#endif