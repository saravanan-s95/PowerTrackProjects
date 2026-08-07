#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"
// inside your_header.h// Replace 'int' with its actual data type
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(argv[2]==NULL)
    {
        return e_failure;
    }
    else
    {
      if(strstr(argv[2],".bmp"))
      {
        decInfo->src_image_fname=argv[2];
      }
       else
       {
         return e_failure;
       }
    }

     if(argv[3]!=NULL)
    {
         decInfo->decode_secret_fname = argv[3];  
    }
    else
    {
        decInfo->decode_secret_fname = "decode";
    }
    
    return e_success;
}
/* Get File pointers for i/p and o/p files */
Status open_decode_file(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");

    if (decInfo->fptr_src_image == NULL)
    {
    	
    	printf("ERROR: Unable to open file %s\n", decInfo->src_image_fname);

    	return e_failure;
    }
    return e_success;
}
Status decode_byte_to_lsb(char *data, char *image_buffer)
{
    *data=0;
    for(int i=0;i<8;i++)
    { 
      *data =  *data<<1;
      *data = (image_buffer[i]&1) | *data;
    }
    return e_success;
}
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image)
{
   
   char image_buffer[8];
   for(int i=0;i<size;i++)
   {
    //step1 : read 8 bytes from the src image[beautiful]
    fread(image_buffer,sizeof(char),8,fptr_src_image);
    decode_byte_to_lsb(&data[i],image_buffer);
   // fwrite(image_buffer,sizeof(char),8,fptr_stego_image);
    //step 3: write 8 bytes to stego image
   }
   return e_success;
}
Status decode_size_to_lsb(int *data,char *image_buffer)
{
     for(int i=0;i<32;i++)
    { 
      *data = (*data<<1);
      *data = (image_buffer[i]&1) | (*data);
      
    }
    return e_success;
}
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    //Skipping first 54 bytes
  fseek(decInfo->fptr_src_image, 54,SEEK_SET);
  char image_buffer[16];
  char dmagic_string[3]={0};
  //decode_data_to_image(dmagic_string,2,decInfo->fptr_src_image)
  fread(image_buffer,sizeof(char),16,decInfo->fptr_src_image);
   for(int i=0;i<8;i++)
    { 
      dmagic_string[0]= (dmagic_string[0]<<1);
      dmagic_string[0] = (image_buffer[i]&1) | (dmagic_string[0]);
      
    }
     for(int i=8;i<16;i++)
    { 
      dmagic_string[1] = (dmagic_string[1]<<1);
      dmagic_string[1] = ((image_buffer[i]&1) | dmagic_string[1]);
      
    }
    dmagic_string[2]='\0';
    if(strcmp(dmagic_string,magic_string)){
        printf("\tMagic string is not matching\n");
        return e_failure;
    }
    
    return e_success;
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{

  char image_buffer[32];
  fread(image_buffer,sizeof(char),32,decInfo->fptr_src_image);
  decode_size_to_lsb((&decInfo->secret_file_extn_size),image_buffer);
  return e_success;
  
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
  // char imagebuffer[decInfo->secret_file_extn_size];
  decode_data_to_image(decInfo->extn_secret_file,decInfo->secret_file_extn_size,decInfo->fptr_src_image);
  decInfo->extn_secret_file[decInfo->secret_file_extn_size]='\0';
  // printf("\t---------> %s\n",decInfo->extn_secret_file);
  char outputfile[50]="";
  strcat(outputfile,decInfo->decode_secret_fname);
  strcat(outputfile,decInfo->extn_secret_file);
   decInfo->fptr_decode_txt = fopen(outputfile, "w");
  return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo){
       
  char image_buffer[32];
  fread(image_buffer,sizeof(char),32,decInfo->fptr_src_image);
  decode_size_to_lsb(&decInfo->size_secret_file,image_buffer);
  // printf("\t--------->%d \n",decInfo->size_secret_file);
  return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
   char imagebuffer[decInfo->size_secret_file];
  
   decode_data_to_image(imagebuffer,decInfo->size_secret_file,decInfo->fptr_src_image);
   fwrite(imagebuffer,sizeof(char),decInfo->size_secret_file,decInfo->fptr_decode_txt);
  return e_success;
}
Status do_decoding(DecodeInfo *decInfo){
   if(open_decode_file(decInfo)==e_failure){
      return e_failure;
   }else{
      printf("\tfile is opened succesfully\n");
      if(decode_magic_string(MAGIC_STRING,decInfo)==e_failure)
      {
        return e_failure;
      }
      else
      {
        printf("\tMagic string decoded and validated successfully\n");
          if(decode_secret_file_extn_size(decInfo)==e_failure)
          {
             return e_failure;
          }else{
            printf("\tsize of the secret file extn  successfully read \n");
          
            if(decode_secret_file_extn(decInfo)==e_failure){
              return e_failure;
            }else{
                printf("\tSecret file extn successfully read\n");
                 printf("\t---------> %s\n",decInfo->extn_secret_file);
                  if(decode_secret_file_size(decInfo)==e_failure){
                     return e_failure;
                  }else{
                    printf("\tsize of the secretfile successfully read\n");
                    printf("\t--------->%d \n",decInfo->size_secret_file);
                   if(decode_secret_file_data(decInfo)==e_failure){
                     return e_failure;
                   }else{
                       printf("\tsecret message successfully read and stored in stored in decoded output file\n");
                   }
                  }
               }
            }
          }
      }
   return e_success;
}