#ifndef ENCODER_H
#define ENCODER_H

#include "common.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Encoder
{
private:

    /* Source Image Information */
    string srcImageName;
    ifstream srcImage;

    /* Secret File Information */
    string secretFileName;
    ifstream secretFile;

    string secretExtension;
    unsigned int secretExtensionSize;
    unsigned long int secretFileSize;

    /* Stego Image Information */
    string outputImageName;
    ofstream outputImage;


public:

    /* Constructor */
    Encoder(string sourceImage, string secretFileName,string outputImage);

    /* Destructor */
    ~Encoder();


    /* Validate input arguments */
    Status validateArguments();


    /* Open source, secret and output files */
    Status openFiles();


    /* Check whether image has enough capacity */
    Status checkCapacity();


    /* Get image size for BMP */
    unsigned int getImageSize();


    /* Get secret file size */
    unsigned int getSecretFileSize();


    /* Copy BMP header */
    Status copyBmpHeader();


    /* Encode magic string */
    Status encodeMagicString(const string &magicString);


    /* Encode secret file extension */
    Status encodeSecretFileExtension();


    /* Encode secret file size */
    Status encodeSecretFileSize();


    /* Encode secret file data */
    Status encodeSecretFileData();


    /* Encode one byte into LSB */
    Status encodeByteToLSB(char data, char *buffer);


    /* Encode four bytes into LSB */
    Status encodeIntToLSB(int data, char *buffer);


    /* Copy remaining image data */
    Status copyRemainingImageData();


    /* Complete encoding process */
    Status encode();
};

#endif