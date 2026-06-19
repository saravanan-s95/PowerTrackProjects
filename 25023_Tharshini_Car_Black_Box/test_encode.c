/*NAME        : THARSHINI S
  DATE        : 17-10-2025
  DESCRIPTION : LSB IMAGE STEGANOGRAPHY -> This project performs encoding and decoding operations on bitmap images */
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{
   EncodeInfo E1;//structure to hold encode info
   DecodeInfo D1;//structure to hold decode info
   //validate arguments
   if(argc<3)
   {
    printf("Invalid Input !\nFor Encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\nFor Decoding : ./a.out -d stego.bmp  [decode.txt]\n ");
    return 1;
   }
   int ret=check_operation_type(argv);
 
   if(ret == e_encode)
   {
     printf("<<<<<Selected Encoding>>>>>\n");
     if(read_and_validate_encode_args(argv,&E1)==e_success)
     {
      printf("Read and validation of encode arguments are success!\n");
      //perform encoding
     if(do_encoding(&E1)==e_success)
     {
      printf("Info : Encoding is success!\n");
     }
     else 
     {
       printf("ERROR : Failed to encode !\n");
       return 1;
     }
    }
     else
     {
      printf("Read and validation of encode arguments are failure!\n");
      return 1;
     }
    }
   else if(ret == e_decode)
   {
     printf("<<<<<Selected Decoding>>>>>\n");
     if(read_and_validate_decode_args(argv,&D1)==d_success)
     {
      printf("Read and validation of decode arguments are success!\n");
      //perform decoding
      if(do_decoding(&D1)==d_success)
      {
        printf("Info : Decoding is success!\n");
      }
      else
      {
        printf("ERROR : Failed to decode\n");
      }
     }
      else
     {
      printf("Read and validation of decode arguments are failure!\n");
      return 1;
     }
   }
   else
   {
     printf("Invalid Input !\n");
   }

    return 0;
}



  //Function to check the operation type based on Command line argument 
   OperationType check_operation_type(char *argv[])
   {
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
   }