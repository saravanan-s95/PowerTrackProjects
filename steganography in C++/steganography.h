#ifndef STEGO_H
#define STEGO_H

#include<iostream>
#include<fstream>
#include<string>

#define MAGIC_STRING  "#*~"

class Steganography
{
    protected:
        std::fstream imageFile;
        std::fstream secretFile;
        std::fstream outputImage;
        std::fstream outputFile;
        
    public:
    Steganography()
    {
    }
    
    bool read_and_validate_encode_args(char *argv[]);
    bool read_and_validate_decode_args(char *argv[]);
};

#endif