/*NAME        : THARSHINI S
  DATE        : 27-05-2026
  DESCRIPTION : PORTING STEGANOGRAPHY TO C++ - Implemented image steganography in C++ to hide and retrieve secret data inside BMP images.*/
  
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types


#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

class Decode
{
    public:
    /*stego image info*/
    string stego_image_fname;
    FILE *fptr_stego_image;
    
    /*output file*/
    string output_fname;
    FILE *fptr_output;
    
    /*secret file Info*/
    string extn_secret_file;
    long size_secret_file;
    long size_extn;

    //Constructor
    Decode(string stego, string output);

    /*perform the decoding process*/
    Status do_decoding();
    
    /*open input/output files required for decoding*/
    Status open_decode_files();
    
    /*decode the actual magic string*/
    Status magic_string();
    
    /*decode a size(byte) value from LSB's of image data*/
    Status decode_size_from_lsb(FILE *fptr, long *size);
    
    /*extract data from image*/
    Status decode_data_from_image(FILE *fptr, string &data, int size);
    
    /*decode the secret file extension size*/
    Status decode_secret_file_extn_size();
    
    /*decode the secret file extension*/
    Status decode_secret_file_extn();
    
    /*decode the secret file size*/
    Status decode_secret_file_size();
    
    /*decode the secret file data from the stego image*/
    Status decode_secret_file_data();
};

#endif