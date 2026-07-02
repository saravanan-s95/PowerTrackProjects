#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types
using namespace std;

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

class Encode
{
    public:

    /* Source Image info */
    string src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    string secret_fname;
    FILE *fptr_secret;
    string extn_secret_file;
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;

    /* Stego Image Info */
    string stego_image_fname;
    FILE *fptr_stego_image;

    //constructor
    Encode(string src, string secret, string stego);

    /* Perform the encoding */
    Status do_encoding();

    /* open files */
    Status open_files();
    
    /* check capacity */
    Status check_capacity();
    
    /* Get image size */
    uint get_image_size_for_bmp(FILE *fptr_image);
    
    /* Get file size */
    uint get_file_size(FILE *fptr);

    /* Copy bmp image header */
    Status copy_bmp_header();
    
    /* Store Magic String */
    Status encode_magic_string(string magic_string);
    
    /*Encode secret file extn size*/
    Status encode_secret_file_extn_size(int size);
    
    /* Encode secret file extenstion */
    Status encode_secret_file_extn(string file_extn);
    
    /* Encode secret file size */
    Status encode_secret_file_size(long file_size);
    
    /* Encode secret file data*/
    Status encode_secret_file_data();
    
    /* Encode function, which does the real encoding */
    Status encode_data_to_image(string data, int size);
    
    /* Encode a byte into LSB of image data array */
    Status encode_byte_to_lsb(char data, char *image_buffer);
    
    /*Encode size to lsb*/
    Status encode_size_to_lsb(int size);
    
    /* Copy remaining image bytes from src to stego image after encoding */
    Status copy_remaining_img_data();
};

#endif