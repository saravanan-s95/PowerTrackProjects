#ifndef DECODE_H
#define DECODE_H

#include <iostream>
#include <fstream>
#include <string>

class Decode
{
private:

    // File names
    std::string encodedImageName;
    std::string outputFileName;

    // File objects
    std::ifstream encodedImage;
    std::ofstream outputFile;

    int extensionSize;
    int secretFileSize;

    // Helper functions
    bool openFiles();

    void skipBmpHeader();

    char decodeByteFromLSB();

    void decodeDataFromImage(char *data, int size);

    void decodeMagicString();

    void decodeSecretExtensionSize();

    void decodeSecretExtension();

    void decodeSecretFileSize();

    void decodeSecretData();

public:

    Decode(std::string image, std::string output);

    void doDecoding();
};

#endif