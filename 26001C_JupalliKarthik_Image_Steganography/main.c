/*
Name : Jupalli Karthik
Date : 15-06-2026
Title : LSB Image Steganography
Description : LSB (Least Significant Bit) Image Steganography is a technique used to hide secret information 
              inside a digital image without noticeably changing its appearance.In this project, the least significant
              bits of the image pixels are modified to store the bits of a secret message.
              Since changing the LSB affects the pixel value very slightly, the changes are usually invisible to the human eye.
*/
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "colors.h"

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{
    EncodeInfo enc;
    DecodeInfo dec;
    if(argc < 2)//The count of arguments should be greater than 2 as per requirement
    {
        printf(RED"For encoding 4 arguments is mandatory and 5th is optional as [./a.out -e .bmp .secret optional].\n"RESET);
        printf(RED"For decoding 3 arguments is mandatory 4th is optional[./a.out -d .bmp optional]\n"RESET);
        return 0;
    }
    OperationType n = check_operation_type(argv[1]);//Passing 1 command line variable and checking whether the user entered encode or deccode
    if(n == e_encode)//This performs encoding operation if user enter second command line argument as -e. it means encode
    {
        if(argc != 4 && argc != 5){//To encode the count of argument should be 4 or 5 like 5th is optional argument
            printf(RED"Insufficient arguments for encoding.\n"RESET);
            return 0;
        }
        if(read_and_validate_encode_args(argv, &enc) == e_failure){//To validate the command line arguments
            printf(RED"Encoding is not possible.\n"RESET);
            return 0;
        }
        if(do_encoding(&enc) == e_failure){//Here the actual encoding logic is written
            printf(RED"Encoding failed.\n"RESET);
            return 0;
        }
    }
    else if(n == e_decode){//This performs decoding operation if user enter second command line argument as -d. it means decode
        if(argc != 3 && argc !=4){//For decoding three arguments is compulsory and 4th is optional
            printf(RED"Insufficient arguments for decoding.\n"RESET);
            return 0;
        }
        if(read_and_validate_decode_args(argc,argv,&dec)==e_failure){// To validate the decode arguments
            printf(RED"Decoding is not possible.\n"RESET);
            return 0;
        }
        if(do_decoding(&dec)==e_failure){
            printf(RED"Decoding failed.\n"RESET);
            return 0;
        }
    }
    else{
        printf(RED"Unsupported operation.\n"RESET);
    }
    return 0;
}
OperationType check_operation_type(char *symbol)//It is used to check the user entered argument is for encode or decode
{
    int n=strcmp(symbol,"-e");
    if(n==0){
        return e_encode;
    }
    int m=strcmp(symbol,"-d");
    if(m==0){
        return e_decode;
    }
    return e_unsupported;
}
