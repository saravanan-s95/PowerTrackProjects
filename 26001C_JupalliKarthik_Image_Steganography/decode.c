#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include"common.h"
#include"colors.h"
/* Function Definitions */
/*This is to validate the decoding arguments and stroing in the struct member*/
Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *dec){
    char *ptr=strstr(argv[2],".bmp");
    if(ptr != NULL){
        dec->stego_name=argv[2];
    }
    else{
        printf("Unable to open the file.\n");
        return e_failure;
    }
    if(argc==4){
        if(argv[3]!=NULL){
            char *ptr1=strtok(argv[3],".");
            strcpy(dec->output_fname,ptr1);
        }
    }
    else{
        strcpy(dec->output_fname,"default");
    }
    return e_success;
}


/*This is to open the bmp file*/
Status open_file_bmp(DecodeInfo *dec){
    FILE *fptr = fopen(dec->stego_name,"r");
    if(fptr != NULL){
        dec->fptr_stego_image=fptr;
    }
    else{
        printf("Error occured in opening BMP file.\n");
        return e_failure;
    }
    return e_success;
}


/*To skip the bmp header because in header we did not encoded anything*/
Status skip_bmp_header(FILE *fptr_stego_image){
    if(fseek(fptr_stego_image,54,SEEK_SET)!=0){
        return e_failure;
    }
    return e_success;
}


/*To decode the magic string and after decoding it ask input from user if user enter the same magic string then it will continue*/
Status decode_magic_string(DecodeInfo *dec){
    char magic[3];
    for(int i=0;i<2;i++){
        char buffer[8];
        fread(buffer,8,1,dec->fptr_stego_image);
        magic[i]=decode_lsb_to_byte(buffer);
    }
    magic[2]='\0';
    char ch[5];
    printf("Enter Magic string : ");
    scanf("%s",ch);
    if(strcmp(magic,ch)!=0){
        printf("Enter valid magic string.\n");
        return e_failure;
    }
    return e_success;
}


/*To decode the secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *dec){
    int n=0;
    char buffer[32];
    fread(buffer,32,1,dec->fptr_stego_image);
    decode_lsb_to_size(&n,buffer);
    dec->extn_size=n;
    return e_success;
}


/*To decode the secret file extension*/
Status decode_secret_file_extn(DecodeInfo *dec){
    char ch[dec->extn_size+1];
    for(int i=0;i<dec->extn_size;i++){
        char buffer[8];
        fread(buffer,8,1,dec->fptr_stego_image);
        ch[i]=decode_lsb_to_byte(buffer);
    }
    ch[dec->extn_size]='\0';
    strcat(dec->output_fname,ch);
    return e_success;
}


/*To open the secret file*/
Status open_file_secret(DecodeInfo *dec){
    FILE *fptr = fopen(dec->output_fname,"w");
    if(fptr!=NULL){
        dec->fptr_output=fptr;
    }
    else{
        return e_failure;
    }
    return e_success;
}


/*To decode the secret file size*/
Status decode_secret_file_size(DecodeInfo *dec){
    unsigned int n=0;
    char buffer[32];
    fread(buffer,32,1,dec->fptr_stego_image);
    decode_lsb_to_size(&n,buffer);
    dec->output_file_size=n;
    return e_success;
}


/*To decode secret file data*/
Status decode_secret_file_data(DecodeInfo *dec){
    char ch;
    for(int i=0;i<dec->output_file_size;i++){
        char buffer[8];
        fread(buffer,8,1,dec->fptr_stego_image);
        ch=decode_lsb_to_byte(buffer);
        fputc(ch,dec->fptr_output);
    }
    return e_success;
}


/*To decode character which is size of 1 byte*/
char decode_lsb_to_byte(char *image_buffer){
    char ch=0;
    for(int i=0;i<8;i++){
        if(image_buffer[i]&1){
            ch = ch|(1<<(7-i));
        }
    }
    return ch;
}


/*To decode the size which is int ->4 bytes*/
Status decode_lsb_to_size(unsigned int *size,char *image_buffer){
    int val=0;
    for(int i=0;i<32;i++){
        if(image_buffer[i]&1){
            val = val |(1<<(31-i));
        }
    }
    *size=val;
    return e_success;
}


Status do_decoding(DecodeInfo *dec)
{
    printf(CYAN "\n========================================\n" RESET);
    printf(CYAN "         DECODING PROCESS STARTED       \n" RESET);
    printf(CYAN "========================================\n\n" RESET);
    
    /*To open the bmp file*/
    if(open_file_bmp(dec) == e_success)
    {
        printf(GREEN "✓ BMP file opened successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to open the BMP file.\n" RESET);
        return e_failure;
    }
    /*
    *Decoding Process * : 
    1-> We are skipping 54 bytes of data because it is header
    2-> Decoding 16 bytes to get 2 bytes magic string if user enter the same magic string then it will proceed further
    3-> Decoding next 32 bytes to get secret file extension size 
    4-> Decoding extension size *8 bytes to fetch the extension of secret file
    5-> Decoding next 32 bytes because to get the size of secret data
    6-> Decoding the data from the file
    7-> Completed decoding process and it gives one output file with secret file extension*/
    /*To skip the BMP header*/
    if(skip_bmp_header(dec->fptr_stego_image) == e_success)
    {
        printf(GREEN "✓ BMP header skipped successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to skip the BMP header.\n" RESET);
        return e_failure;
    }
    /*To decode the magic string*/
    if(decode_magic_string(dec) == e_success)
    {
        printf(GREEN "✓ Magic string verified successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Magic string verification failed.\n" RESET);
        return e_failure;
    }
    /*To decode the secret file extension size */
    if(decode_secret_file_extn_size(dec) == e_success)
    {
        printf(GREEN "✓ Secret file extension size decoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to decode the extension size.\n" RESET);
        return e_failure;
    }
    /*To decode the secret file extension*/
    if(decode_secret_file_extn(dec) == e_success)
    {
        printf(GREEN "✓ Secret file extension decoded successfully.\n" RESET);
        printf(YELLOW "[INFO] Output file : %s\n" RESET,dec->output_fname);
    }
    else
    {
        printf(RED "✗ Failed to decode the file extension.\n" RESET);
        return e_failure;
    }
    /*To open the secret file*/
    if(open_file_secret(dec) == e_success)
    {
        printf(GREEN "✓ Output file opened successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to open the output file.\n" RESET);
        return e_failure;
    }
    /*TO decode the secret file size*/
    if(decode_secret_file_size(dec) == e_success)
    {
        printf(YELLOW "[INFO] Secret file size : %ld bytes\n" RESET,
               dec->output_file_size);
        printf(GREEN "✓ Secret file size decoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to decode the secret file size.\n" RESET);
        return e_failure;
    }
    /*To decode the secret file data*/
    if(decode_secret_file_data(dec) == e_success)
    {
        printf(GREEN "✓ Secret file data decoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to decode the secret file data.\n" RESET);
        return e_failure;
    }

    printf(CYAN "\n========================================\n" RESET);
    printf(GREEN "      DECODING COMPLETED SUCCESSFULLY   \n" RESET);
    printf(CYAN "========================================\n\n" RESET);

    return e_success;
}