#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

class Encoder
{
    /* Source Image info */
    string src_image_fname;
    ifstream fptr_src_image;
    uint image_capacity = 0;
    char image_data[8] = {};

    /* Secret File Info */
    string secret_fname;
    ifstream fptr_secret;
    string extn_secret_file;
    char secret_data = 0;
    uint size_secret_file = 0;

    /* Stego Image Info */
    string stego_image_fname;
    ofstream fptr_stego_image;

    /* Get File pointers for i/p and o/p files */
    Status open_files();
    
    /* check capacity */
    Status check_capacity();
    
    /* Get image size */
    uint get_image_size_for_bmp();
    
    /* Get file size */
    uint get_file_size();
    
    /* Copy bmp image header */
    Status copy_bmp_header();
    
    /* Store Magic String */
    Status encode_magic_string(const string &magic_string);
    
    /* Encode secret file extension size */
    Status encode_secret_file_extn_size();
    
    /* Encode secret file extension */
    Status encode_secret_file_extn();
    
    /* Encode secret file size */
    Status encode_secret_file_size();
    
    /* Encode secret file data*/
    Status encode_secret_file_data();
    
    /* Encode a byte into LSB of image data array */
    Status encode_byte_to_lsb(char data,char* image_buffer);
    
    /* Copy remaining image bytes from src to stego image after encoding */
    Status copy_remaining_img_data();

    /* Close all files */
    void close_files();

    public:
        /* Encoding function prototype */
        
        /* Read and validate Encode args from argv */
        Status read_and_validate_encode_args(char *argv[]);
        
        /* Perform the encoding */
        Status do_encoding();
};

#endif
