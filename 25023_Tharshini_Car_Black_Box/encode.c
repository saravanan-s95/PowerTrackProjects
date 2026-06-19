/*NAME        : THARSHINI S
  DATE        : 17-10-2025
  DESCRIPTION : LSB IMAGE STEGANOGRAPHY */
  
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //beautiful.bmp
    if(argv[2]!=NULL && strstr(argv[2],".bmp")!=NULL && strcmp(strstr(argv[2],".bmp"),".bmp")==0)
    {
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        printf("ERROR : Source image must be a .bmp file!\n");
        return e_failure;
    }

    //secret.txt
    if(argv[3]!=NULL && strstr(argv[3],".txt")!=NULL && strcmp(strstr(argv[3],".txt"),".txt")==0)
    {
        encInfo->secret_fname=argv[3];
    }
    else
    {
       printf("ERROR : Secret file must be a .txt file!\n");
       return e_failure;
    }

    if(argv[4]!=NULL && strstr(argv[4],".bmp")!=NULL && strcmp(strstr(argv[4],".bmp"),".bmp")==0)
    {
       //validate .bmp or not
       encInfo->stego_image_fname =argv[4];
    }
    else
    {
      encInfo->stego_image_fname="default.bmp";
    }
    return e_success;

}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    
    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)
{
  encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
  encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
  if(encInfo->image_capacity > 16+32+32+32+ encInfo->size_secret_file*8)
  {
    return e_success;
  }
  else
  {
    return e_failure;
  }
}

//Copy the first 54 bytes of BMP header from source image to stego image
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char bmp_header[54];
    rewind(fptr_src_image);
    fread(bmp_header,54,1,fptr_src_image);
    fwrite(bmp_header,54,1,fptr_dest_image);
    return e_success;
}

//Encodes a single byte into the LSBs of 8 image bytes
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i] &0xFE)|((data >>i) & 0x01);
    }
    return e_success;    
}

//Encodes a string of data into the image using LSB technique
Status encode_data_to_image(char *data, int size,EncodeInfo *encInfo)
{
    for(int i=0;i<size;i++)
    {
    //read 8 bytes from src file
    fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
    //to perform encoding pass 1 char and 8 bytes of data
    encode_byte_to_lsb(data[i],encInfo->image_data);
    //write encoded 8 bytes to output file
    fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

//Encodes predefined magic string to identify stego image
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
   encode_data_to_image(magic_string,strlen(magic_string),encInfo);
   return e_success;
}

//Encode a 32-bit integer into LSBs of 32 image bytes
Status encode_size_to_lsb(int size,EncodeInfo *encInfo)
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    //to encode int-32bits only in lsb
    for(int i=0;i<32;i++)
    {
        str[i]=(str[i] &0xFE)|((size >>i) & 0x01);//0 1 2 3 4 5 6 7 
    }
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//Encode the length of the secret file extension
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    encode_size_to_lsb(size,encInfo);
    return e_success;
}

//Encode the secret file extension
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
encode_data_to_image(file_extn,strlen(file_extn),encInfo);
return e_success;
}

//Encode the size of the secret file
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size,encInfo);
    return e_success;
}

//Encode the actual content of the secret file
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    fgets(data,encInfo->size_secret_file,encInfo->fptr_secret);
    encode_data_to_image(data,encInfo->size_secret_file,encInfo);
    return e_success;
}

//Copy the remaining image data after encoding
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    int size;
    size=encInfo->image_capacity + 54 - ftell(encInfo->fptr_src_image);
    char remaining_data[size];
    fread(remaining_data,size,1,encInfo->fptr_src_image);
    fwrite(remaining_data,size,1,encInfo->fptr_stego_image);
    return e_success;
}

//encoding process is performed here
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo)==e_success)
    {
        printf("Info : Files opened successfully...\n");
        if(check_capacity(encInfo)==e_success)
        {
            printf("Secret data can fit into the given image file\n");
            if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
            {
                printf("Copied BMP header successfully\n");
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                    printf("Encoded magic string successfully\n");
                    strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname,"."));
                    printf("%s\n",encInfo->extn_secret_file);
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
                    {
                        printf("Encoded secret file extension size successfully\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
                        {
                            printf("Encoded secret file extension successfully\n");
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf("Encoded secret file size successfully\n");
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("Encoded secret file data successfully\n");
                                    if(copy_remaining_img_data(encInfo)==e_success)
                                    {
                                        printf("Copied remaining data successfully\n");
                                    }
                                    else
                                    {
                                        printf("Failed to copy remaining data\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Failed to encode secret file data\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Failed to encode secret file size\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to encode secret file extension\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to encode secret file extension size\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to encode magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Failed to copy BMP header\n");
                return e_failure;
            }
        }
        else
        {
            printf("cannot fit Secret data in the given image file...\n");
        }
    }
    else
    {
        printf("ERROR : Failed to open files\n");
        return e_failure;
    }
    return e_success;//all steps are successfull
}