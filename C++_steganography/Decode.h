#ifndef DECODER_H
#define DECODER_H

#include "common.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Decoder
{
private:

    /* Stego Image Information */
    string stegoImageName;
    ifstream stegoImage;


    /* Secret File Information */
    string secretFileName;
    ofstream secretFile;

    string secretExtension;
    unsigned int secretExtensionSize;
    unsigned long int secretFileSize;


public:

    /* Constructor */
    Decoder(string stegoImage, string outputFile);


    /* Destructor */
    ~Decoder();


    /* Validate input arguments */
    Status validateArguments();


    /* Open stego image */
    Status openImageFile();


    /* Decode magic string */
    Status decodeMagicString();


    /* Decode secret file extension */
    Status decodeSecretFileExtension();


    /* Decode secret file size */
    Status decodeSecretFileSize();


    /* Decode secret file data */
    Status decodeSecretFileData();


    /* Decode one byte from LSB */
    char decodeByteFromLSB(char *buffer);


    /* Decode four bytes from LSB */
    int decodeIntFromLSB(char *buffer);


    /* Complete decoding process */
    Status decode();
};

#endif