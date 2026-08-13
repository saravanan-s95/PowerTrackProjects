#ifndef DECODE_H
#define DECODE_H

#include<iostream>
#include<fstream>
#include<string>
#include "steganography.h"

using namespace std;

class Decoder : public Steganography
{
private:
    // std::fstream outputFile;
    std::string stegoImageName;
    std::string outputFileName;

    int secretSize;
    int extension_size;
    string extension;
    
public:
    Decoder(string image,string output)
    {
        stegoImageName = image;
        outputFileName = output;
    }

    bool decode();

    bool openFiles();

    bool decodeMagicString();
    bool decodeExtensionSize();
    bool decodeExtension();
    bool decodeSecretSize();
    bool decodeSecretData();

    bool decode_byte_from_lsb(char *data, char *buffer);
    bool decode_int_from_lsb(int *size, char *buffer);
};

#endif