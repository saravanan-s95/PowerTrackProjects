#include <iostream>
#include "types.h"
#include "encode.h"
#include "decode.h"
#include "common.h"

using namespace std;

int main(int argc,char* argv[])
{
    //Check argc > 2
    if(argc <= 2)
    {
        cout << "Error : Too few arguments\n";
        return 0;
    }

    //Check whether encoding or decoding
    if(check_operation_type(argv) == e_encode)
    {
        //Declare encode object variable 
        Encoder encode;
        //Read and validate command line arguments for encoding
        if(encode.read_and_validate_encode_args(argv) == e_success)
        {
            //Perform encoding
            cout << "Read and validate encode args function is successful\n";
        }
        else
        {
            //Failure
            cout << "Read and validate encode args function failed\n";
            return e_failure;
        }
        //Perform encoding
        if(encode.do_encoding() == e_success)
        {
            //Success
            cout << "Encoding successful\n";
            return e_success;
        }
        else
        {
            //Failure
            cout << "Encoding Failed\n";
            return e_failure;
        }
    }
    else if(check_operation_type(argv) == e_decode)
    {
        //Declare decode object variable
        Decoder decode;
        //Read and validate command line arguments for decoding
        if(decode.read_and_validate_decode_args(argv) == e_success)
        {
            //Perform decoding
            cout << "Read and validate decode args function is successful\n";
        }
        else
        {
            //Failure
            cout << "Read and validate decode args function failed\n";
            return e_failure;
        }
        //Perform decoding
        if(decode.do_decoding() == e_success)
        {
            //Success
            cout << "Decoding successful\n";
            return e_success;
        }
        else
        {
            //Failure
            cout << "Decoding Failed\n";
            return e_failure;
        }
    }
    else
    {
        //Print error
        //STOP
        cout << "The entered option is invalid\n";
        return e_failure;
    }
    
    return 0;
}