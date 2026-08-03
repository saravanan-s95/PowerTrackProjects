#ifndef DECODE_H
#define DECODE_H

#include "types.h"

/* Structure for decoding */
typedef struct _DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char output_fname[30];
    FILE *fptr_output;

    int extn_size;
    char extn[10];

    int size_secret_file;

} DecodeInfo;


/* Function prototypes */

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_decode_files(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status skip_bmp_header(FILE *fptr);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

Status decode_data_from_image(FILE *fptr, char *data, int size);

char decode_byte_from_lsb(char *buffer);

int decode_int_from_lsb(FILE *fptr);

#endif