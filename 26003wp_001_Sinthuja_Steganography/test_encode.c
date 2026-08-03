#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc,char **argv)
{
    if(check_operation_type(argv)==e_encode)
    {
        printf("Selected Encoding\n");
        EncodeInfo encoding;
        if(read_and_validate_encode_args(argv,&encoding) ==e_success)
        {
            printf("Read and validate encode arg is a success\n");
            printf("------------Started encoding--------------\n");
            if(do_encoding(&encoding) == e_success)
            {
                printf("Encoding is successfully done \n");
            }
            else{
                printf("Failed to encode the data\n");
                return 1;
            }
        }
        else
        {
            printf("Read and validate encode arg is a failure\n");
            return 1;
        }
    }
    else if(check_operation_type(argv)==e_decode)
    {
        printf("Selected Decoding\n");
        DecodeInfo decoding;
        if(read_and_validate_decode_args(argv,&decoding) == e_success)
    {
        printf("Read and validate decode args success\n");
        printf("-----------Started Decoding-----------\n");

        if(do_decoding(&decoding) == e_success)
        {
            printf("Decoding completed successfully\n");
        }
        else
        {
            printf("Decoding failed\n");
            return 1;
        }
    }
    else
    {
        printf("Invalid decode arguments\n");
        return 1;
    }
}
    
else
    {
        printf("Invalid arguments\n");
    }

    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e")==0)
    return e_encode;
    else if(strcmp(argv[1],"-d")==0)
    return e_decode;
    else
    return e_unsupported;
}
