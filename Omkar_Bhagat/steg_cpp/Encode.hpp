#ifndef ENCODE_HPP
#define ENCODE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>

#include "types.hpp"
#include "common.hpp"

using namespace std;

class Encode
{
private:
    /* Source Image */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;

    /* Secret File */
    char *secret_fname;
    FILE *fptr_secret;

    char extn_secret_file[10];
    long size_secret_file;

    /* Stego Image */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    unsigned char image_data[8];

public:
    Encode();

    Status readAndValidateArgs(char *argv[]);

    Status openFiles();

    uint getImageSizeForBmp();

    uint getFileSize(FILE *fptr);

    Status checkCapacity();

    Status copyBmpHeader();

    Status encodeMagicString();

    Status encodeSecretFileExtnSize();

    Status encodeSecretFileExtn();

    Status encodeSecretFileSize();

    Status encodeSecretFileData();

    Status encodeDataToImage(char *data, int size);

    Status encodeByteToLSB(char data,
                           char *image_buffer);

    Status encodeSizeToLSB(int size);

    Status copyRemainingImageData();

    Status doEncoding();
};

#endif