
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "encode.h"
#include "common.h"

using namespace std;

uint get_image_size_for_bmp(ifstream &fptr_image)
{
    uint width, height;

    fptr_image.seekg(18, ios::beg);

    fptr_image.read(reinterpret_cast<char*>(&width), sizeof(int));
    fptr_image.read(reinterpret_cast<char*>(&height), sizeof(int));

    return width * height * 3;
}

Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image.open(encInfo->src_image_fname, ios::in | ios::binary);

    if (!encInfo->fptr_src_image.is_open())
    {
        return e_failure;
    }

    encInfo->fptr_secret.open(encInfo->secret_fname, ios::in | ios::binary);

    if (!encInfo->fptr_secret.is_open())
    {
        return e_failure;
    }

    encInfo->fptr_stego_image.open(encInfo->stego_image_fname, ios::out | ios::binary);

    if (!encInfo->fptr_stego_image.is_open())
    {
        return e_failure;
    }

    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strstr(argv[2], ".bmp") == NULL)
    {
        return e_failure;
    }

    char *extn = strrchr(argv[3], '.');

    if (extn == NULL || strlen(extn) != 4)
    {
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];
    encInfo->secret_fname = argv[3];
    encInfo->stego_image_fname = "stego.bmp";

    strcpy(encInfo->extn_secret_file, extn);

    return e_success;
}

uint get_file_size(ifstream &fptr)
{
    fptr.seekg(0, ios::end);

    uint size = fptr.tellg();

    fptr.seekg(0, ios::beg);

    return size;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    if (encInfo->image_capacity >= (14 + encInfo->size_secret_file) * 8)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(EncodeInfo *encInfo)
{
    char header[54];

    encInfo->fptr_src_image.seekg(0, ios::beg);

    encInfo->fptr_src_image.read(header, 54);

    encInfo->fptr_stego_image.write(header, 54);

    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        encInfo->fptr_src_image.read(encInfo->image_data, 8);

        encode_byte_to_lsb(magic_string[i], encInfo->image_data);

        encInfo->fptr_stego_image.write(encInfo->image_data, 8);
    }

    return e_success;
}

Status encode_data_to_image(const char *data, int size, EncodeInfo *encInfo)
{
    for (int i = 0; i < size; i++)
    {
        encInfo->fptr_src_image.read(encInfo->image_data, 8);

        encode_byte_to_lsb(data[i], encInfo->image_data);

        encInfo->fptr_stego_image.write(encInfo->image_data, 8);
    }

    return e_success;
}

Status encode_extension_size(EncodeInfo *encInfo)
{
    uint extn_size = strlen(encInfo->extn_secret_file);

    return encode_data_to_image(reinterpret_cast<const char*>(&extn_size),
                                 sizeof(extn_size), encInfo);
}

Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    return encode_data_to_image(encInfo->extn_secret_file,
                                strlen(encInfo->extn_secret_file), encInfo);
}

Status encode_secret_file_size(EncodeInfo *encInfo)
{
    return encode_data_to_image(reinterpret_cast<const char*>(&encInfo->size_secret_file),
                                sizeof(encInfo->size_secret_file), encInfo);
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    vector<char> buffer(encInfo->size_secret_file);

    encInfo->fptr_secret.read(buffer.data(), buffer.size());

    return encode_data_to_image(buffer.data(), buffer.size(), encInfo);
}

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    char ch;

    while (encInfo->fptr_src_image.get(ch))
    {
        encInfo->fptr_stego_image.put(ch);
    }

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (check_capacity(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (copy_bmp_header(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        return e_failure;
    }

    if (encode_extension_size(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (encode_secret_file_extn(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (encode_secret_file_size(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (encode_secret_file_data(encInfo) == e_failure)
    {
        return e_failure;
    }

    if (copy_remaining_img_data(encInfo) == e_failure)
    {
        return e_failure;
    }

    encInfo->fptr_src_image.close();
    encInfo->fptr_secret.close();
    encInfo->fptr_stego_image.close();

    return e_success;
}
