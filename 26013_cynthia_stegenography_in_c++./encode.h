#ifndef ENCODE_H
#define ENCODE_H

#include <fstream>
#include "types.h"
#include "common.h"

struct EncodeInfo
{
    const char *src_image_fname;
    std::ifstream fptr_src_image;

    const char *secret_fname;
    std::ifstream fptr_secret;

    const char *stego_image_fname;
    std::ofstream fptr_stego_image;

    char extn_secret_file[5];
    char image_data[8];

    uint image_capacity;
    uint size_secret_file;
};

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

uint get_image_size_for_bmp(std::ifstream &fptr_image);

Status open_files(EncodeInfo *encInfo);

uint get_file_size(std::ifstream &fptr);

Status check_capacity(EncodeInfo *encInfo);

Status copy_bmp_header(EncodeInfo *encInfo);

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

Status encode_data_to_image(const char *data, int size, EncodeInfo *encInfo);

Status encode_byte_to_lsb(char data, char *image_buffer);

Status encode_extension_size(EncodeInfo *encInfo);

Status encode_secret_file_extn(EncodeInfo *encInfo);

Status encode_secret_file_size(EncodeInfo *encInfo);

Status encode_secret_file_data(EncodeInfo *encInfo);

Status copy_remaining_img_data(EncodeInfo *encInfo);

Status do_encoding(EncodeInfo *encInfo);

#endif