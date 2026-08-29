#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include"common.h"
#include"colors.h"
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
    printf(BLUE "Width = %u\n" RESET, width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf(BLUE "Height = %u\n" RESET, height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)//To know the secret file size
{
    fseek(fptr,0,SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)//Validating encoding arguments 
{
    char *ptr = strstr(argv[2], ".bmp");//Checking whether bmp is present or not
    if(ptr == NULL || strcmp(ptr, ".bmp") != 0)
    {
        printf(RED "Please enter a valid source BMP file.\n" RESET);
        return e_failure;
    }
    encInfo->src_image_fname = argv[2];// If it is valid then storing in the structure member
    char *ptr1 = strchr(argv[3], '.');//Checking whether for secret file extension is there or not
    if(ptr1 == NULL)
    {
        printf(RED "The secret file must have an extension.\n" RESET);
        return e_failure;
    }
    encInfo->secret_fname = argv[3];//If extension is there then storing in the structure member
    strcpy(encInfo->extn_secret_file,ptr1);//To store the extension of secret file
    if(argv[4] != NULL)//Checking whether user is entered ouptut file or not
    {
        ptr = strstr(argv[4], ".bmp");//If yes it is checking .bmp extension
        if(ptr != NULL && strcmp(ptr, ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];//If it is present then storing in the structure member
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";//If user is not entered optional file then we are creating one default output file
    }

    return e_success;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }
    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
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

Status check_capacity(EncodeInfo *encInfo)//Storing the file size in structure members for bmp file and secret file
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);//For bmp file
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);//For secret file
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)// To copy the 54 bytes of bmp header to the ouput file
{
    rewind(fptr_src_image);
    char buffer[54];//Creating a buffer of size 54 bytes
    fread(buffer,54,1,fptr_src_image);//Reading 54 bytes from the source file
    fwrite(buffer,54,1,fptr_dest_image);//Writing 54 bytes to the output file
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)//To encode magic string
{
    for(int i=0;magic_string[i]!='\0';i++){//To encode 1 character -> 8 bytes is required from source file because we are doing in onlu lsb position of each byte
        char buffer[8];//Creating character buffer with size 8
        fread(buffer,8,1,encInfo->fptr_src_image);//Reading * bytes of data from bmp or src file
        encode_byte_to_lsb(magic_string[i],buffer);//Calling encode_byte_to_lsb function beacuse character is 1 byte so...
        fwrite(buffer,8,1,encInfo->fptr_stego_image);//after encoding writing into the dest file
    }
    return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)//To encode extension size of secret file
{
    char buffer[32];//The extension size is int so int is 32 bytes.so,buffer is creating with size of 32 bytes
    fread(buffer,32,1,encInfo->fptr_src_image);//Reading 32 bytes from src file
    encode_size_to_lsb(size,buffer);//calling encode_size_to_lsb function
    fwrite(buffer,32,1,encInfo->fptr_stego_image);//Writing 32 bytes encoded data to the dest file
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)//To encode secret file extension
{
    for(int i=0;file_extn[i]!='\0';i++){//Running a loop upto null character
        char buffer[8];//The extension is in character so creating a buffer with size of 8
        fread(buffer,8,1,encInfo->fptr_src_image);//Reading 8 bytes from src file
        encode_byte_to_lsb(file_extn[i],buffer);//Calling encode_byte_to_lsb fun because it is character and size is 1 byte
        fwrite(buffer,8,1,encInfo->fptr_stego_image);//Writing to the output file after encoding
    }
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)//To encode total size of the secret file
{
    char buffer[32];//Size is int and it is 32 bytes 
    fread(buffer,32,1,encInfo->fptr_src_image);//Reading 32 bytes of data from src file
    encode_size_to_lsb(file_size,buffer);//calling encode_size_to_lsb fun
    fwrite(buffer,32,1,encInfo->fptr_stego_image);//storing data to the output file
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)// To encode secret file data
{
    for(int i=0;i<encInfo->size_secret_file;i++){//Running a loop upto secret file size times
        char ch1=fgetc(encInfo->fptr_secret);//Fetching 1 character from secret file 
        char buffer[8];//Creating a buffer of size 8
        fread(buffer,8,1,encInfo->fptr_src_image);//Reading 8bytes form src file
        encode_byte_to_lsb(ch1,buffer);//Calling encode function 
        fwrite(buffer,8,1,encInfo->fptr_stego_image);//Writing to the dest file
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)//To copy the remaining data of src file to the dest file to look as same 
{
    char buffer[1024];
    size_t bytes;

    while((bytes = fread(buffer,1,sizeof(buffer),fptr_src)) > 0)//Fetching bytes of data
    {
        fwrite(buffer,1,bytes,fptr_dest);//Writing to the dest file
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)//To encode character or 8 bytes to lsb
{
    for(int i=0;i<8;i++){//The size of buffer is 8 asusual char size is 8
        image_buffer[i]=image_buffer[i] & ~(1);//Clearing the lsb position of all characters in buffer 
    }
    for(int i=0;i<8;i++){
        char ch1 = data & (1<<(7-i));//Fetching bits from MSB
        if(ch1){//If the bit is set then performing set operation
            image_buffer[i]=image_buffer[i]|1;
        }
    }
    return e_success;
}

Status encode_size_to_lsb(int size, char *imageBuffer)//To encode size which is int
{
    for(int i=0;i<32;i++){//The size of buffer is 32 asusual int size is 32
        imageBuffer[i]=imageBuffer[i]&~1;//Clearing the lsb position of all characters in buffer
    }
    for(int i=0;i<32;i++){
        if((size) & (1<<(31-i))){//Fetching bits from MSB
            imageBuffer[i]=imageBuffer[i]|1;//If it is 1 then setting the bit
        }
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)//DO encoding function
{
    printf(CYAN "\n========================================\n" RESET);
    printf(CYAN "         ENCODING PROCESS STARTED       \n" RESET);
    printf(CYAN "========================================\n" RESET);
    // encInfo->
    // Opening files
    if(open_files(encInfo) == e_failure)
    {
        printf(RED "✗ Failed to open the required files.\n" RESET);
        return e_failure;
    }
    else
    {
        printf(GREEN "✓ All files were opened successfully.\n" RESET);
    }

    // Comparing whether the BMP file has enough capacity
    if(check_capacity(encInfo) == e_failure)
    {
        printf(RED "✗ Failed to check image capacity.\n" RESET);
        return e_failure;
    }

    unsigned long int size = get_file_size(encInfo->fptr_secret);

    int required = (strlen(MAGIC_STRING) * 8) +32 +(strlen(encInfo->extn_secret_file) * 8) +
                   32 +(size * 8);
    /*
    *Encoding process* : 
    1-> copying 54 bytes of BMP header
    2-> Encoding magic string it is a collection of characters to encode each character 8 bytes is required
    3-> Encoding secret file extn size it is int 32 bytes is required
    4-> Encoding secret file extn it is collection of characters to encode this extnsize * 8 bytes is required
    5-> Encoding secret file size it is in int 4 bytes to encode it 32 bytes is required
    6-> Encoding secret file data to encode this size of secret file * 8 bytes is required
    7-> Copying the remaining data from src file to dest file
    8-> Completed encoding process and it gives one .bmp file as ouptut
    */

    unsigned long int size1 = get_image_size_for_bmp(encInfo->fptr_src_image);
    if(size1 < required)
    {
        printf(RED "✗ The BMP image does not have enough capacity to encode the secret data.\n" RESET);
        return e_failure;
    }
    else
    {
        printf(GREEN "✓ Image capacity verification completed successfully.\n" RESET);
    }

    // Copy BMP header
    if(copy_bmp_header(encInfo->fptr_src_image,
                       encInfo->fptr_stego_image) == e_success)
    {
        printf(GREEN "✓ The BMP header was copied successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to copy the BMP header.\n" RESET);
        return e_failure;
    }

    // Encode magic string
    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
        printf(GREEN "✓ The magic string was encoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to encode the magic string.\n" RESET);
        return e_failure;
    }

    // Encode secret file extension size
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),
                                    encInfo) == e_success)
    {
        printf(GREEN "✓ The secret file extension size was encoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to encode the secret file extension size.\n" RESET);
        return e_failure;
    }
    // Encode secret file extension
    if(encode_secret_file_extn(encInfo->extn_secret_file,
                               encInfo) == e_success)
    {
        printf(GREEN "✓ The secret file extension was encoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to encode the secret file extension.\n" RESET);
        return e_failure;
    }

    // Encode secret file size
    if(encode_secret_file_size(encInfo->size_secret_file,
                               encInfo) == e_success)
    {
        printf(GREEN "✓ The secret file size was encoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to encode the secret file size.\n" RESET);
        return e_failure;
    }

    // Encode secret file data
    if(encode_secret_file_data(encInfo) == e_success)
    {
        printf(GREEN "✓ The secret file data was encoded successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to encode the secret file data.\n" RESET);
        return e_failure;
    }
    // Copy remaining image data
    if(copy_remaining_img_data(encInfo->fptr_src_image,
                               encInfo->fptr_stego_image) == e_success)
    {
        printf(GREEN "✓ The remaining image data was copied successfully.\n" RESET);
    }
    else
    {
        printf(RED "✗ Failed to copy the remaining image data.\n" RESET);
        return e_failure;
    }

    printf(CYAN "\n========================================\n" RESET);
    printf(GREEN "      ENCODING COMPLETED SUCCESSFULLY   \n" RESET);
    printf(CYAN "========================================\n\n" RESET);

    return e_success;
}