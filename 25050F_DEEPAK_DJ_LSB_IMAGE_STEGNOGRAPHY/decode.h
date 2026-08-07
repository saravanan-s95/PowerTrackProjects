#ifndef DECODE_H
#define DECODE_H
#include "types.h" // Contains user defined type
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 5

typedef struct _DecodeInfo
{
   
    char *src_image_fname; //store the src file name
    FILE *fptr_src_image; //file pointer
   
     char extn_secret_file[MAX_FILE_SUFFIX];
   
       int secret_file_extn_size;
       int size_secret_file;

    /* Stego Image Info */
    char *decode_secret_fname; // store the output file name
    FILE *fptr_decode_txt; //file pointer
} DecodeInfo;
// inside your_header.h


/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_file(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
Status decode_secret_filsize(DecodeInfo *decInfo);
/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image);

/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *data, char *image_buffer);

#endif
