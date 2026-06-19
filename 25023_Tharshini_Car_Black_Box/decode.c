/*NAME        : THARSHINI S
  DATE        : 17-10-2025
  DESCRIPTION : LSB IMAGE STEGANOGRAPHY */
  
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    //stego.bmp
    if(argv[2]!=NULL && strstr(argv[2],".bmp")!=NULL && strcmp(strstr(argv[2],".bmp"),".bmp")==0)
    {
        decInfo->stego_image_fname=argv[2];
    }
    else
    {
        printf("ERROR : Stego image must be a .bmp file!\n");
        return d_failure;
    }
    //output file
    if(argv[3]!=NULL && strstr(argv[3],".txt")!=NULL && strcmp(strstr(argv[3],".txt"),".txt")==0)
    {
        decInfo->output_fname=argv[3];
    }
    else
    {
      decInfo->output_fname="decode.txt";
    }
    return d_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
    	return d_failure;
    }

     // output file
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output== NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        fclose(decInfo->fptr_stego_image);
    	return d_failure;
    }
    return d_success;   
}

Status magic_string(DecodeInfo *decInfo)
{
    char magic[4];
    char buf[8];
    rewind(decInfo->fptr_stego_image);
    fseek(decInfo->fptr_stego_image,54,SEEK_SET); //skip bmp header
    //Read the magic string length of bytes after skipping the BMP header
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        fread(buf,8,1,decInfo->fptr_stego_image);
        magic[i]=0;
        for(int j=0;j<8;j++)
        {
            int bit=buf[j]&0x01;
            magic[i]=magic[i] | (bit<<j);
        }
    }
    if(strcmp(magic,MAGIC_STRING)==0)
    {
        printf("Magic string is matched\n");
        return d_success;
    }
    else
    {
        printf("ERROR : Magic string is not matched\n");
        return d_failure;
    }
}

Status decode_size_from_lsb(FILE *fptr, long *size)
{
    *size=0;
     char buffer[32];
    fread(buffer,32,1,fptr);
     for(int i=0;i<32;i++)
     {
        int bit=buffer[i]&0x01;//extract lsb
        *size=*size | (bit<<i);//set bit in size at position i
     }
     return d_success;
}


Status decode_data_from_image(FILE *fptr, char *data, int size)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(buffer,8,1,fptr);
        data[i]=0;
        for(int j=0;j<8;j++)
        {
            int bit=buffer[j]&0x01;//extract lsb
            data[i]=data[i] | (bit<<j); //combine bits into byte
        }
    }
    return d_success;
}


Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(decInfo->fptr_stego_image,&decInfo->size_extn);
    return d_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->fptr_stego_image,decInfo->extn_secret_file,decInfo->size_extn);
    return d_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(decInfo->fptr_stego_image,&decInfo->size_secret_file);
    return d_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char *secret;
    secret=malloc(decInfo->size_secret_file+1); //allocate the buffer dynamically for secret data
    if(secret==NULL)
    {
        printf("ERROR : Memory allocation failed\n");
        return d_failure;
    }
    decode_data_from_image(decInfo->fptr_stego_image,secret,decInfo->size_secret_file);
    secret[decInfo->size_secret_file]='\0';
    fwrite(secret,decInfo->size_secret_file,1,decInfo->fptr_output);
    free(secret);
    return d_success;
}

//Decoding process
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo)==d_success)
    {
        printf("Info : Files opened successfully...\n");
        if(magic_string(decInfo)==d_success)
        {
            printf("Magic string is decoded successfully\n");
            if(decode_secret_file_extn_size(decInfo)==d_success)
            {
                 printf("Decoded secret file extension size successfully\n");
                 if(decode_secret_file_extn(decInfo)==d_success)
                 {
                     printf("Decoded secret file extension successfully\n");
                     if(decode_secret_file_size(decInfo)==d_success)
                     {
                         printf("Decoded secret file size is successfully\n");
                         if(decode_secret_file_data(decInfo)==d_success)
                         {
                            printf("Decoded secret file data sucessfully\n");
                         }
                         else
                         {
                            printf("Failed to decode secret data\n");
                            return d_failure;
                         }

                     }
                     else
                     {
                        printf("Failed to decode secret file size\n");
                        return d_failure;
                     }
                 }
                 else
                 {
                    printf("Failed to decode secret file extension\n");
                    return d_failure;
                 }
            }
            else
            {
                printf("Failed to decode secret file extension size\n");
                return d_failure;
            }
        }
        else
        {
            printf("Magic string is mismatch\n");
            return d_failure;

        }
     
    }
    else
    {
        printf("ERROR : Failed to open files\n");
        return d_failure;
    }
    return d_success;
}