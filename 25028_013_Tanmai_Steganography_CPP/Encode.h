#ifndef ENCODE_H
#define ENCODE_H

#include <iostream>
#include <fstream>
#include <string>
#include "Types.h"
#include "Common.h"

using namespace std;

/*---------------------------------------------------------------
    Class Name : Encode

    Description :
        This class performs the complete encoding process.
        It hides the contents of a secret text file inside
        a BMP image using the Least Significant Bit (LSB)
        Steganography technique.
----------------------------------------------------------------*/

class Encode
{
private:

    /* Source BMP Image Name */
    string srcImageName;

    /* Secret Text File Name */
    string secretFileName;

    /* Output Stego Image Name */
    string stegoImageName;

    /* File Streams */
    ifstream srcImage;
    ifstream secretFile;
    ofstream stegoImage;

    /* Stores Image Capacity */
    long imageCapacity;

    /* Stores Secret File Size */
    long secretFileSize;

public:

    /* Constructor */
    Encode(string src, string secret, string stego);

    /* Opens Source Image, Secret File and Stego Image */
    Status openFiles();

    /* Checks whether image has enough capacity */
    Status checkCapacity();

    /* Copies 54-byte BMP Header */
    Status copyBMPHeader();

    /* Encodes one character into 8 image bytes */
    void encodeByteToLSB(char data, char *buffer);

    /* Encodes an integer into 32 image bytes */
    void encodeIntToLSB(int data, char *buffer);

    /* Encodes Magic String into Image */
    Status encodeMagicString();

    /* Encodes Secret File Size */
    Status encodeSecretFileSize();

    /* Encodes Secret File Data */
    Status encodeSecretData();

    /* Copies Remaining Image Data */
    Status copyRemainingData();

    /* Performs Complete Encoding Process */
    Status doEncoding();
};

#endif