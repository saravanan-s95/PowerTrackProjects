#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>

#include "types.h" // Contains user defined types

typedef struct _DecodeInfo
{
    /* Source Image info */
    char *stego_name;
    FILE *fptr_stego_image;   

    /* Secret File Info */
    char output_fname[100];       
    FILE *fptr_output;  
    int extn_size;   
    char extn_secret_file[20]; 
    char secret_data[100];
    long output_file_size;   

} DecodeInfo;

/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file_bmp(DecodeInfo *decInfo);

/* Skip bmp image header */
Status skip_bmp_header(FILE *fptr_stego_image);

/*To open output file with extension */
Status open_file_secret(DecodeInfo *decInfo);
/* Get Magic String */
Status decode_magic_string(DecodeInfo *decInfo);
/* after decode magic string read the magic string from decoder and compare 
if same continue the decoding 
else stop the decoding */

/*Decode extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);
/*after decode extn merge the default name with extension and open the output file */

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a lsb of buffer into 1byte */
char decode_lsb_to_byte(char *image_buffer);

// Encode a lsb of buffer into 4 byte
Status decode_lsb_to_size(unsigned int *size, char *imageBuffer);

#endif
