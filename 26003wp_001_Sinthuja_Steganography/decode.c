#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"

/* Read and validate args */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (argv[2] != NULL && strcmp(strchr(argv[2], '.'), ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        strcpy(decInfo->output_fname, argv[3]);
    }
    else
    {
        strcpy(decInfo->output_fname, "output");
    }

    return e_success;
}

/* Open files */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}

/* Skip 54 bytes */
Status skip_bmp_header(FILE *fptr)
{
    fseek(fptr, 54, SEEK_SET);
    return e_success;
}

/* Decode 1 byte */
char decode_byte_from_lsb(char *buffer)
{
    char data = 0;

    for (int i = 0; i < 8; i++)
    {
        data |= (buffer[i] & 1) << (7 - i);
    }

    return data;
}

/* Decode data */
Status decode_data_from_image(FILE *fptr, char *data, int size)
{
    char buffer[8];

    for (int i = 0; i < size; i++)
    {
        fread(buffer, 8, 1, fptr);
        data[i] = decode_byte_from_lsb(buffer);
    }

    return e_success;
}

/* Decode int */
int decode_int_from_lsb(FILE *fptr)
{
    char buffer[32];
    int data = 0;

    fread(buffer, 32, 1, fptr);

    for (int i = 0; i < 32; i++)
    {
        data |= (buffer[i] & 1) << (31 - i);
    }

    return data;
}

/* Step 2: Magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic[3];

    decode_data_from_image(decInfo->fptr_stego_image, magic, 2);
    magic[2] = '\0';

    if (strcmp(magic, MAGIC_STRING) == 0)
    {
        printf("Magic string matched\n");
        return e_success;
    }
    else
    {
        printf("Magic string not found\n");
        return e_failure;
    }
}

/* Step 3: extension size */
Status decode_extn_size(DecodeInfo *decInfo)
{
    decInfo->extn_size = decode_int_from_lsb(decInfo->fptr_stego_image);
    return e_success;
}

/* Step 4: extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->fptr_stego_image, decInfo->extn, decInfo->extn_size);
    decInfo->extn[decInfo->extn_size] = '\0';

    strcat(decInfo->output_fname, decInfo->extn);

    return e_success;
}

/* Step 5: file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->size_secret_file = decode_int_from_lsb(decInfo->fptr_stego_image);
    return e_success;
}

/* Step 6: secret data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");

    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    char buffer[8];
    char ch;

    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(buffer);
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    printf("Secret data decoded into %s\n", decInfo->output_fname);

    return e_success;
}

/* Main decode flow */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
        return e_failure;

    skip_bmp_header(decInfo->fptr_stego_image);

    if (decode_magic_string(decInfo) == e_failure)
        return e_failure;

    printf("Step 1: Magic string OK\n");

    decode_extn_size(decInfo);
    printf("Step 2: Extension size = %d\n", decInfo->extn_size);

    decode_secret_file_extn(decInfo);
    printf("Step 3: Extension = %s\n", decInfo->extn);

    decode_secret_file_size(decInfo);
    printf("Step 4: File size = %d\n", decInfo->size_secret_file);

    if (decode_secret_file_data(decInfo) == e_failure)
        return e_failure;

    printf("Step 5: Data decoded successfully\n");

    return e_success;
}