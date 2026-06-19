/*NAME        : THARSHINI S
  DATE        : 17-10-2025
  DESCRIPTION : LSB IMAGE STEGANOGRAPHY */
  
#ifndef DECODE_H
#define DECODE_H

#include "types.h"//contains user defined datatypes
#include<stdio.h>

/*
 * Structure to store information required for
 * decoding data from a stego image
 * Info about output/input file, magic string
 * secret file extension and secret data size
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
  /*stego image info*/
  char *stego_image_fname;
  FILE *fptr_stego_image;

  /*output file*/
  char *output_fname;
  FILE *fptr_output;

  /*secret file Info*/
  char extn_secret_file[MAX_FILE_SUFFIX];
  long size_secret_file;
  long size_extn;
}DecodeInfo;

/*Decoding functions protoype*/

/*read and validate command-line args for decoding*/
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/*perform the decoding process*/
Status do_decoding(DecodeInfo *decInfo);

/*open input/output files required for decoding*/
Status open_decode_files(DecodeInfo *decInfo);

/*decode the actual magic string*/
Status magic_string(DecodeInfo *decInfo);

/*decode a size(byte) value from LSB's of image data*/
Status decode_size_from_lsb(FILE *fptr, long *size);

/*extract data from image*/
Status decode_data_from_image(FILE *fptr, char *data, int size);

/*decode the secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*decode the secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*decode the secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/*decode the secret file data from the stego image*/
Status decode_secret_file_data(DecodeInfo *decInfo);


#endif