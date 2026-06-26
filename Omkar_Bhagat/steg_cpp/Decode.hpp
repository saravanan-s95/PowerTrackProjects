#ifndef DECODE_HPP
#define DECODE_HPP

#include <iostream>
#include <cstdio>
#include <cstring>

#include "types.hpp"
#include "common.hpp"

using namespace std;

class Decode
{
private:
    /* Input Stego Image */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output File */
    char *output_fname;
    FILE *fptr_output_file;

    char extn_secret_file[10];
    int extn_size;
    long size_secret_file;

    unsigned char image_data[8];

public:
    Decode();

    Status readAndValidateArgs(char *argv[]);

    Status openFiles();

    Status decodeByteFromLSB(unsigned char *image_buffer,
                             unsigned char *data);

    Status decodeDataFromImage(char *data,
                               int size);

    Status decodeMagicString();

    Status decodeSecretFileExtnSize();

    Status decodeSecretFileExtn();

    Status decodeSecretFileSize();

    Status decodeSecretFileData();

    Status doDecoding();
};

#endif