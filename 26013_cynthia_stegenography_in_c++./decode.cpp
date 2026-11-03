
#include <iostream>
#include <cstring>
#include <vector>
#include "decode.h"

using namespace std;

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2], ".bmp") == NULL)
    {
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    strcpy(decInfo->output_fname, "output");

    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image.open(decInfo->stego_image_fname, ios::in | ios::binary);

    if (!decInfo->fptr_stego_image.is_open())
    {
        return e_failure;
    }

    return e_success;
}

Status decode_byte_from_lsb(char *image_buffer, char *data)
{
    unsigned char decoded_data = 0;

    for (int i = 0; i < 8; i++)
    {
        decoded_data = decoded_data << 1;
        decoded_data = decoded_data | (image_buffer[i] & 1);
    }

    *data = decoded_data;

    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    for (int i = 0; i < size; i++)
    {
        decInfo->fptr_stego_image.read(decInfo->image_data, 8);

        decode_byte_from_lsb(decInfo->image_data, &data[i]);
    }

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];

    decode_data_from_image(magic_string, 2, decInfo);

    magic_string[2] = '\0';

    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }

    return e_failure;
}

Status decode_extension_size(DecodeInfo *decInfo)
{
    uint extn_size;

    decode_data_from_image(reinterpret_cast<char*>(&extn_size),
                           sizeof(extn_size), decInfo);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->extn_secret_file, 4, decInfo);

    decInfo->extn_secret_file[4] = '\0';

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_data_from_image(reinterpret_cast<char*>(&decInfo->size_secret_file),
                           sizeof(decInfo->size_secret_file), decInfo);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    vector<char> buffer(decInfo->size_secret_file);

    decode_data_from_image(buffer.data(), buffer.size(), decInfo);

    ofstream output_file(decInfo->output_fname, ios::out | ios::binary);

    if (!output_file.is_open())
    {
        return e_failure;
    }

    output_file.write(buffer.data(), buffer.size());

    output_file.close();

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
    {
        return e_failure;
    }

    decInfo->fptr_stego_image.seekg(54, ios::beg);

    if (decode_magic_string(decInfo) == e_failure)
    {
        return e_failure;
    }

    if (decode_extension_size(decInfo) == e_failure)
    {
        return e_failure;
    }

    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        return e_failure;
    }

    if (decode_secret_file_size(decInfo) == e_failure)
    {
        return e_failure;
    }

    if (decode_secret_file_data(decInfo) == e_failure)
    {
        return e_failure;
    }

    decInfo->fptr_stego_image.close();

    return e_success;
}
