#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include<string.h>
// inside your_header.h
//xtern int decode_fileextnsize; // Replace 'int' with its actual data type
OperationType check_operation_type(char *arg)
{

    if(!strcmp(arg,"-e"))
    {
        return e_encode;
    }

    if(!strcmp(arg,"-d"))
    {
        return e_decode;
    }

    return e_unsupported;
}
int main(int argc,char *argv[])
{
    int ret;
    if(argc >1)
        ret = check_operation_type(argv[1]);
    else 
       ret =  3;

    EncodeInfo encInfo;
    DecodeInfo decInfo;
    if(ret==e_encode)
    {
        printf("\tencoding is selected\n");
        //call the read and validate function
        if((read_and_validate_encode_args(argv,&encInfo)) == e_success)
        {
            printf("\tRead and validated sucessfully\n");
            // call a do encoding function
            do_encoding(&encInfo);
        }else{
            printf("\tERROR : Not enough arguments selected\n");
            printf("\tFor Encoding Enter : ./a.out -e image.bmp secret.txt <optional .bmp file output>\n");
            printf("\tFor Decoding Enter : ./a.out -d encoded_file.bmp <optional output file name>\n");
        }
    }
    else if(ret==e_decode)
    {
        printf("\tdecoding is selected\n");
       if((read_and_validate_decode_args(argv,&decInfo)) == e_success)
        {
            printf("\tRead and validated sucessfully\n");
            // call a do encoding function
            do_decoding(&decInfo);
        }else{
            printf("\tERROR : Not enough arguments selected\n");
            printf("\tFor Encoding Enter : ./a.out -e beautiful.bmp secret.txt <optional .bmp file output>\n");
            printf("\tFor Decoding Enter : ./a.out -d encoded_file.bmp <optional output file name>\n");
        }
    }
    else
    {
        printf("\tERROR : Not enough arguments selected\n");
        printf("\tFor Encoding Enter : ./a.out -e beautiful.bmp secret.txt <optional .bmp file output>\n");
        printf("\tFor Decoding Enter : ./a.out -d encoded_file.bmp <optional output file name>\n");
    }
    
    return 0;
}
