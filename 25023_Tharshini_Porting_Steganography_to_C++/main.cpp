/*NAME        : THARSHINI S
  DATE        : 27-05-2026
  DESCRIPTION : PORTING STEGANOGRAPHY TO C++ - Implemented image steganography in C++ to hide and retrieve secret data inside BMP images.*/
  
#include "types.h"
#include "encode.h"
#include "decode.h"

using namespace std;
//Function to check the operation type based on Command line argument 
OperationType check_operation_type(char *argv[])
{
   if(string(argv[1])=="-e") //check for encoding option
   {
      return e_encode;
   }
   else if(string(argv[1])=="-d") //check for decoding option
   {
      return e_decode;
   }
   else //unsupported option
   {
      return e_unsupported;
   }
}

int main(int argc,char *argv[])
{
   //validate command line arguments
   if(argc<3)
   {
    cout<<"Invalid Input !"<<endl;
    cout<<"For Encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]"<<endl;
    cout<<"For Decoding : ./a.out -d stego.bmp  [decode.txt]"<<endl;
    return 1;
   }

   //determine operation type
   OperationType ret;
   ret=check_operation_type(argv);

   //encoding operation
   if(ret==e_encode)
   {
     cout<<"<<<<<Selected Encoding>>>>>"<<endl;

     string stego_fname;

     if(argv[4]!=NULL)
     {
        stego_fname=argv[4]; //use user provided output stego image name
     }
     else
     {
        stego_fname="stego.bmp"; //default stego image name
     }

     //create encode obj
     Encode enc(argv[2],argv[3],stego_fname);

     //perform encoding
     if(enc.do_encoding()==e_success)
     {
        cout<<"Info : Encoding is success!"<<endl;
     }
    else 
     {
       cout<<"ERROR : Failed to encode !"<<endl;
       return 1;
     }
   }

   //decoding operation
   else if(ret==e_decode)
   {
     cout<<"<<<<<Selected Decoding>>>>>"<<endl;

   string output_file;

   if(argv[3]!=NULL)
   {
      output_file=argv[3];//use user provided output file name
   }
   else
   {
      output_file="decode.txt";//default decoded output file name
   }

   //create decode obj
   Decode dec(argv[2],output_file);

   //perform decoding
   if(dec.do_decoding()==d_success)
   {
      cout<<"Info : Decoding is success!"<<endl;
   }
   else
   {
      cout<<"ERROR : Failed to decode"<<endl;
      return 1;
   }
   }
   return 0;
}
