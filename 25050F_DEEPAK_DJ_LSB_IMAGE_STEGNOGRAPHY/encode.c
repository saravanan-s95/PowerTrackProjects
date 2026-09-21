#include<stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"
/* Function Definitions */
// inside your_header.h
//extern int decode_fileextnsize; // Replace 'int' with its actual data type
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
    printf("\twidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("\theight = %u\n", height);

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
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname,"w");
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
    // step 1 check argv[2] is .bmp file or not
    if(argv[2]==NULL){
        return e_failure;
    }else{

      if(strstr(argv[2],".bmp"))
      {
        encInfo->src_image_fname=argv[2];
      }
       else
       {
         return e_failure;
       }
    }
    if(argv[3]==NULL){
        return e_failure;
    }else{

     if(strstr(argv[3],".txt"))
      {
        encInfo->secret_fname=argv[3];
        char *ptr=strrchr(argv[3],'.');
         strcpy(encInfo->extn_secret_file,ptr);
        // printf("------> %s ",encInfo->extn_secret_file);
        // fetch the sec extn 
        // extn_secret file[] store it arr
      }
      else
      {
         return e_failure;
      }
    }

    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp"))
       {
        encInfo->stego_image_fname=argv[4];
       }
        else
        {
            return e_failure;
        }
    }
    else
    {
       encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    // step 1 : move the file pointer to end 
    fseek(fptr,0,SEEK_END);
    return (uint)ftell(fptr);
    // step 2 : return the size by using f tell
}
Status check_capacity(EncodeInfo *encInfo)
{
   // header - 54 bytes;
   // after that encode magic string - #* 16 (bytes)
   // 70 - bytes 
   // magic string is a data only encoder and decoder knows that 
   // so it acts like a password
   // here we will encode first sizeof extn and then we will encode extn
   // if .txt then we will encode in integer form 4*8 = 32 bytes 
   // sizeof  extn is a number so integer form which is nothing but 
   // binary 32 - bits -> takes 32 bytes
   // after that we will encode extention of secret file
   // extn -> depends on input
   // sizeof(extn) * 8
   // secret file size -> integer form 
   // secret file data 
     int src_file_size = get_image_size_for_bmp(encInfo->fptr_src_image);

     encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

     if(src_file_size > 54 + strlen(MAGIC_STRING)+ 32 + strlen(encInfo->extn_secret_file)*8 + 32 + encInfo->size_secret_file*8){
        return e_success;
     }
     else{
        return e_failure;
     }
 }
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    char bmpheader[54];
    rewind(fptr_src_image);
    fread(bmpheader,sizeof(char),54,fptr_src_image);
    fwrite(bmpheader,sizeof(char),54,fptr_dest_image);
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    { 
      int get=((data>>(7-i))&1);
      image_buffer[i] = (image_buffer[i]&0xFE);//clear
      image_buffer[i] = (image_buffer[i])| get;
    }
    return e_success;
}
Status encode_size_to_lsb(int data,char *image_buffer)
{
     for(int i=0;i<32;i++)
    { 
      int get=((data>>(31-i))&1);
      image_buffer[i] = (image_buffer[i]&0xFE);//clear
      image_buffer[i] = (image_buffer[i])|get;
    }
    return e_success;
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
   
   char image_buffer[8];
   for(int i=0;i<size;i++)
   {
    //step1 : read 8 bytes from the src image[beautiful]
    fread(image_buffer,sizeof(char),8,fptr_src_image);
    encode_byte_to_lsb(data[i],image_buffer);
    fwrite(image_buffer,sizeof(char),8,fptr_stego_image);
    //step 3: write 8 bytes to stego image
   }
   return e_success;
}
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
 {
    encode_data_to_image(magic_string,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
 }

Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
  //encode secret file extension size 
  char image_buffer[32];
  // step 1 : read 32 bytes from the src [beautiful.bmp]
   fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image);
  //step 2 call encode size to lsb
   encode_size_to_lsb(file_extn_size,image_buffer);
   fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image);
  // write 32 bytes to stego image 
  return e_success;
}
Status encode_secret_file_extn(char *extn,EncodeInfo *encInfo)
{
  encode_data_to_image(extn,strlen(extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
  return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
  //encode secret file extension size 
  char image_buffer[32];
  // step 1 : read 32 bytes from the src [beautiful.bmp]
  fread(image_buffer,sizeof(char),32,encInfo->fptr_src_image);
  //step 2 call encode size to lsb(filesize,image buffer)
  encode_size_to_lsb(file_size,image_buffer);
  // write 32 bytes to stego image 
   fwrite(image_buffer,sizeof(char),32,encInfo->fptr_stego_image);
  return e_success;
}
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo){
    char data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
    // read a contents form the secret file[secret.txt] and store in array
    fread(data,sizeof(char),encInfo->size_secret_file,encInfo->fptr_secret);
    encode_data_to_image(data,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    // read byte by byte from src till end of the file
    while(!feof(fptr_src))
    {
        fread(&ch,sizeof(char),1,fptr_src);
       // write byte by byte to dest
       fwrite(&ch,sizeof(char),1,fptr_dest);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
   // all other related to encoding functions are called in this function only
   if((open_files(encInfo))==e_failure){
            printf("\tERROR : UNABLE TO OPEN ALL FILES\n");
            return e_failure;
   }
   else
   {
     printf("\tFILES are opened Sucessfully\n");
     if(check_capacity(encInfo)==e_failure){
        printf("error msg");
     }else{
        printf("\tcapacity verified\n");
        if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){
             return e_failure;
        }else{
            printf("\tsucessfully copied bmp header\n");
            if(encode_magic_string("#*",encInfo)==e_failure){
                return e_failure;
            }else{
                printf("\tsuccessfuly encoded magic string\n");
                if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_failure){
                    return e_failure;
                }else{
                    printf("\tsuccessfully encoded file extn size\n");
                    if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_failure){
                        return e_failure;
                    }else{
                        printf("\tsuccessfully encoded file extn\n");
                        if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure)
                        {
                          return e_failure;
                        }else
                        {
                             printf("\tsucessfully encoded secret file size\n");
                             if(encode_secret_file_data(encInfo)==e_failure){
                                return e_failure;
                             }else{
                                printf("\tsucessfully encoded secret file data\n");
                                if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){
                                    return e_failure;
                                }else{
                                    printf("\tsuccessfully copied remaining data\n");
                                }
                             }
                        }
                    }
                }
            }
        }
     }
   }
    return e_success;
}
