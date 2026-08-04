#ifndef ENCODE_H
#define ENCODE_H

#include <iostream>
#include <fstream>
#include <string>

class Encode
{
private:

    // File names
    std::string srcImageName;
    std::string secretFileName;
    std::string outputImageName;

    // File objects
    std::ifstream srcImage;
    std::ifstream secretFile;
    std::ofstream outputImage;

    // Helper Functions
    bool validateFiles();
    bool openFiles();
    bool checkCapacity();

    void copyBmpHeader();

    void encodeByteToLSB(char data);
    void encodeDataToImage(const char *data, int size);
    void encodeMagicString();

    void encodeSecretExtensionSize();
    void encodeSecretExtension();

    void encodeSecretFileSize();
    void encodeSecretData();

    void copyRemainingImageData();

public:

    Encode(std::string src, std::string secret, std::string output);

    void doEncoding();
};

#endif