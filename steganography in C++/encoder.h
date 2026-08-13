#ifndef ENCODE_H
#define ENCODE_H

#include<iostream>
#include<fstream>
#include<string>
#include "steganography.h"

using namespace std;

class Encoder : public Steganography
{
private:
    // std::fstream secretFile;
    // std::fstream outputImage;

    std::string inputImageName;
    std::string secretFileName;
    std::string outputImageName;

    int imageCapacity;
    int secretSize;

    string extension;
    int extensionSize;
public:
    Encoder(string image,string secret,string output)
    {
        inputImageName = image;
        secretFileName = secret;
        outputImageName = output;
    }

    bool encode();

    void get_extension();
    bool openFiles();
    bool checkCapacity();
    bool copyHeader();
    bool encodeMagicString();
    bool encodeExtensionSize();
    bool encodeExtension();
    bool encodeSecretSize();
    bool encodeSecretData();
    bool copyRemainingData();
    
    bool encode_byte_to_lsb(char data, char *buffer);
    bool encode_int_to_lsb(int data,char *buffer);

};

#endif