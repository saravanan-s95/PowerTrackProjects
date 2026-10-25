#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/* 
 * Structure to store information required for
 * decoding encoded ouput file to secret data
 * Info about output and intermediate data is
 * also stored
 */

class Decoder
{
    /* Encoded output file Info */
    string encoded_output_fname;
    ifstream fptr_encoded_output_image;
    char image_data_decode[8] ={};

    /* Secret File Info */
    string secret_fname_decode;
    ofstream fptr_secret_decode;
    string extn_secret_file_decode;
    uint size_extn_file_decode = 0;
    uint file_size_decode = 0;

    /* Get File pointers for i/p and o/p files */
    Status open_files_decode();

    /* Open the secret file */
    Status open_files_decode_file();
    
    /* Skip the header of 54 bytes*/
    Status skip_bmp_header();
    
    /* Store Magic String */
    Status decode_magic_string(const string &magic_string);
    
    /* Decode secret file extension size */
    Status decode_secret_file_extn_size();
    
    /* Decode secret file extension */
    Status decode_secret_file_extn();
    
    /* Decode secret file size */
    Status decode_secret_file_size();
    
    /* Decode secret file data*/
    Status decode_secret_file_data();
    
    /* Decode byte from LSB of encoded output bmp file */
    Status decode_byte_from_lsb(char* data, char* image_buffer);

    /* Close all files */
    void close_files();

    public:
        /* Decoding function prototype */

        /* Read and validate decode args from argv */
        Status read_and_validate_decode_args(char *argv[]);
        
        /* Perform the decoding */
        Status do_decoding();
};

#endif
