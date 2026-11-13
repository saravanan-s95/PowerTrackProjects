#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char **argv)
{
    printf("Lets see what have you selected !!!\n");

    if (check_operation_type(argv) == e_encode)
    {
        printf("Congrats! You have selected encoding\n");

        EncodeInfo steganography;

        if (read_and_validate_encode_args(argv, &steganography) == e_success)
        {
            printf("Read and validated encode arguments successfully\n");
            printf("<-------- Lets start encoding -------->\n");

            if (do_encoding(&steganography) == e_success)
            {
                printf("Completed encoding successfully\n");
            }
            else
            {
                printf("Failed to perform encoding\n");
                return -1;
            }
        }
        else
        {
            printf("Failed to validate encode arguments\n");
            return -1;
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        printf("Congrats! You have selected decoding\n");
        DecodeInfo decInfo;

    if (read_and_validate_decode_args(argv, &decInfo) == e_success)
    {
        printf("Read and validated decode arguments successfully\n");
        printf("<-------- Lets start decoding -------->\n");

        if (do_decoding(&decInfo) == e_success)
        {
            printf("Completed decoding successfully\n");
        }
        else
        {
            printf("Failed to perform decoding\n");
            return -1;
        }
        }
        else
        {
           printf("Failed to validate decode arguments\n");
           return -1;
        }
    }
    else
    {
        printf("Invalid input\n");
        printf("For encoding: ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding: ./a.out -d stego.bmp [output.txt]\n");
    }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}