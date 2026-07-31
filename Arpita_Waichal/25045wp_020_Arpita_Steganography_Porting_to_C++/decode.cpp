#include <iostream>
#include <cstring>
#include "common.h"
#include "decode.h"

Status DecodeInfo::open_files_decode()
{
    
    fptr_stego_image = fopen(stego_src_name, "r");
    if (fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",stego_src_name);

        return e_failure;
    }
    return e_success;
}


Status DecodeInfo::read_and_validate_decode_args(char *argv[])
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        stego_src_name=argv[2];
    }
    else
        return e_failure;

    if(argv[3]==NULL)
    {
        output_fname[0]='o';
        output_fname[1]='u';
        output_fname[2]='t';
        output_fname[3]='p';
        output_fname[4]='u';
        output_fname[5]='t';
        output_fname[6]='\0';
    }
    else
    {   int i = 0;
          for(i=0;i<strlen(argv[3]);i++)
          {	
            output_fname[i]=argv[3][i];
          }

           output_fname[i] = '\0';

           extn_output_file = output_fname;
           return e_success;
    }

    return e_success;
}

Status DecodeInfo::decode_magic_string(const char *data)
{
    char arr[strlen(data)];
    char buffer[8];
    int i;
    
    fseek(fptr_stego_image,54,SEEK_SET);

    for(i=0;i<strlen(data);i++)
    { 
	fread(buffer,8,1,fptr_stego_image);
	
	arr[i]=decode_byte_from_lsb(buffer);
    }
    arr[i]='\0';
    
    if(strcmp(arr,data)==0)
	return e_success;
    else
	return e_failure;
}


char DecodeInfo::decode_byte_from_lsb(char* buffer)
{
    char ch=0;
    for(int i=0;i<8;i++)
    {
	ch=(buffer[i]&1)<<i|ch;
    }
    return ch;
}


Status DecodeInfo::decode_extension_size()
{
   char buffer[32];
   
   fread(buffer,32,1,fptr_stego_image);
   
   int size=decode_size_from_lsb(buffer);

   dsize_secret_file=size;

   return e_success;
}


int DecodeInfo::decode_size_from_lsb(char* buffer)
{
    int size=0;
    for(int i=0;i<32;i++)
    {
	size=(buffer[i]&1)<<i|size;
    }
    return size;
}


Status DecodeInfo::decode_secret_extension()
{
    int i=0;
    char arr[dsize_secret_file+1];
    char buffer[8];

    for(i=0;i<dsize_secret_file;i++)
    {
        fread(buffer,8,1,fptr_stego_image);

 	    arr[i]=decode_byte_from_lsb(buffer);

    }
    arr[i]='\0';

    int len_name = strlen(output_fname);
    int len_ext = strlen(arr);

    if (len_name < len_ext || strcmp(output_fname + len_name - len_ext, arr) != 0)
    {
        strcat(output_fname, arr);
    }

extn_output_file = output_fname;

    return e_success;
}


Status DecodeInfo::decode_secret_file_size()
{
   char buffer[32];
   fread(buffer,32,1,fptr_stego_image);
   
   int size=decode_size_from_lsb(buffer);


   dsize_secret_file_data=size;
 
   return e_success;
}


Status DecodeInfo::decode_secret_file_data()
{
    fptr_output=fopen(output_fname, "w");
    char arr[dsize_secret_file_data];
    char buffer[8];
    int i;
    for(i=0;i<dsize_secret_file_data;i++)
    {
	fread(buffer,8,1,fptr_stego_image);
	
	arr[i]=decode_byte_from_lsb(buffer);
	
	fwrite(&arr[i],1,1,fptr_output);
    }
    return e_success;
}


Status DecodeInfo::do_decoding()
{
    printf("Started Decoding\n");
    if(open_files_decode()==e_success)
    {
        printf("Open files successful\n");
    }
    else
    {
        printf("Open files failure\n");
        return e_failure;
    }
    if(decode_magic_string(password)==e_success)
    {
        printf("Decoding Magic String Successful\n");
    }
    else
    {
	printf("Decoding Magic String Unseccessful\n");
	return e_failure;
    }
    if(decode_extension_size()==e_success)
    {
	printf("Decoding of extension size in Successful\n");
    }
    else
    {
	printf("Decoding of extension size is unSuccessful\n");
    }

    if(decode_secret_extension()==e_success)
    {
	printf("Decoding of secret extension is Successful\n");
    }
    else
    {
	printf("Decoding of secret extension is unsuccessful\n");

    }

    if(decode_secret_file_size()==e_success)
    {
	printf("Decoding of secret file size is successful\n");
    }
    else
    {
       printf("Decoding of secret file size is unsuccessful\n");
    }
    if(decode_secret_file_data()==e_success)
    {
	printf("Decoding of secret file data  is successful\n");
    }
    else
    {
       printf("Decoding of secret file data is unsuccessful\n");
    }


    return e_success;
}
