#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2] == NULL)
    {   
        printf("Stego image not mentioned\n");
        return e_failure;
    }

char *ptr = strrchr(argv[2], '.');

    if (ptr != NULL && strcmp(ptr, ".bmp") == 0)
    {
        printf("Received a proper bmp file\n");
    decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Failed to receive proper bmp file\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        printf("Received output file name\n");
        decInfo->output_fname = argv[3];
    }
    else
    {
        printf("Output file name not mentioned. Creating decoded.txt\n");
        decInfo->output_fname = "decoded.txt";
    }

    return e_success;
}
Status open_decode_files(DecodeInfo *decInfo)
{
    /* Stego Image file */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                decInfo->stego_image_fname);

        return e_failure;
    }

    /* Output file */
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");

    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                decInfo->output_fname);

        return e_failure;
    }

    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    printf("Lets open the files in required mode\n");

    if (open_decode_files(decInfo) == e_success)
    {
        printf("Opened required files in required mode successfully\n");
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }

    printf("Started decoding magic string\n");

    if (decode_magic_string(decInfo) == e_success)
    {
        printf("Decoded magic string successfully\n");
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }
        printf("Started decoding secret file extension size\n");

    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        printf("Decoded secret file extension size successfully\n");
    }
    else
    {
        printf("Failed to decode secret file extension size\n");
        return e_failure;
    }
        printf("Started decoding secret file extension\n");

    if (decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Decoded secret file extension successfully\n");
    }
    else
    {
        printf("Failed to decode secret file extension\n");
        return e_failure;
    }
        printf("Started decoding secret file size\n");

    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf("Decoded secret file size successfully\n");
        printf("Started decoding secret file data\n");

        if (decode_secret_file_data(decInfo) == e_success)
        {
           printf("Decoded secret file data successfully\n");
        }
       else
       {
        printf("Failed to decode secret file data\n");
        return e_failure;
       }
    }
    else
    {
        printf("Failed to decode secret file size\n");
        return e_failure;
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    *data = 0;

    for (int i = 0; i < 8; i++)
    {
        *data = *data << 1;
        *data = *data | (image_buffer[i] & 1);
    }

    return e_success;
}

Status decode_integer_from_lsb(int *data, char *image_buffer)
{
    *data = 0;

    for (int i = 0; i < 32; i++)
    {
        *data = (*data << 1) | (image_buffer[i] & 1);
    }

    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];

    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
           return e_failure;
        }

        if (decode_byte_from_lsb(&data[i], image_buffer) == e_failure)
        {
           return e_failure;
        }
    }

    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];

    /* Skip BMP header */
    if (fseek(decInfo->fptr_stego_image, 54, SEEK_SET) != 0)
    {
    return e_failure;
    }

    /* Decode 2 characters */
    if (decode_data_from_image(magic_string, 2, decInfo) == e_failure)
    {
         return e_failure;
    }

    magic_string[2] = '\0';

    /* Compare with original magic string */
    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    /* Read 32 bytes from stego image */
    if (fread(image_buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        return e_failure;
    }

    /* Decode extension size */
    if (decode_integer_from_lsb(&decInfo->extn_size, image_buffer) == e_failure)
    {
        return e_failure;
    }

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    /* Decode extension (.txt) */
    if (decInfo->extn_size <= 0 ||
    decInfo->extn_size >= sizeof(decInfo->extn_secret_file))
    {
        return e_failure;
    }
    if (decode_data_from_image(decInfo->extn_secret_file,
                           decInfo->extn_size,
                           decInfo) == e_failure)
    {
        return e_failure;
    }

    /* Add null character */
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char image_buffer[32];

    /* Read 32 bytes from stego image */
    if (fread(image_buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        return e_failure;
    }

    /* Decode secret file size */
    if (decode_integer_from_lsb(&decInfo->size_secret_file, image_buffer) == e_failure)
    {
        return e_failure;
    }

    if (decInfo->size_secret_file <= 0)
    {
        return e_failure;
    }

   return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
       if (decode_data_from_image(&ch, 1, decInfo) == e_failure)
        {
            return e_failure;
        }

       if (fputc(ch, decInfo->fptr_output) == EOF)
       {     
            return e_failure;
       }
    }

    if (fclose(decInfo->fptr_stego_image) == EOF)
            return e_failure;

    if (fclose(decInfo->fptr_output) == EOF)
            return e_failure;

    return e_success;
}