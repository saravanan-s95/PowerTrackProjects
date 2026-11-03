
#ifndef DECODE_H
#define DECODE_H

#include <fstream>
#include "types.h"
#include "common.h"

struct DecodeInfo
{
    const char *stego_image_fname;
    std::ifstream fptr_stego_image;

    char extn_secret_file[5];
    char output_fname[100];

    char image_data[8];

    uint size_secret_file;
};

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_decode_files(DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *image_buffer, char *data);

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_extension_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

#endif
