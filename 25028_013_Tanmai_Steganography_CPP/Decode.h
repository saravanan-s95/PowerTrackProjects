#ifndef DECODE_H
#define DECODE_H

#include <iostream>
#include <fstream>
#include <string>
#include "Types.h"
#include "Common.h"

using namespace std;

/*---------------------------------------------------------------
    Class Name : Decode

    Description :
        This class performs the complete decoding process.
        It extracts the hidden secret text file from the
        stego BMP image using the Least Significant Bit
        (LSB) Steganography technique.
----------------------------------------------------------------*/

class Decode
{
private:

    /* Stego BMP Image Name */
    string stegoImageName;

    /* Output Decoded Text File Name */
    string outputFileName;

    /* File Streams */
    ifstream stegoImage;
    ofstream outputFile;

    /* Stores the Size of the Secret File */
    int secretFileSize;

public:

    /* Constructor */
    Decode(string stego, string output);

    /* Opens Stego Image and Output File */
    Status openFiles();

    /* Decodes One Character from 8 Image Bytes */
    char decodeByteFromLSB(char *buffer);

    /* Decodes Integer from 32 Image Bytes */
    int decodeIntFromLSB(char *buffer);

    /* Verifies the Magic String */
    Status decodeMagicString();

    /* Decodes Secret File Size */
    Status decodeSecretFileSize();

    /* Decodes Secret File Data */
    Status decodeSecretData();

    /* Performs Complete Decoding Process */
    Status doDecoding();
};

#endif