#include<iostream>
#include"decoder.h"
#include"encoder.h"
#include"steganography.h"
#include<cstring>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        cout<<"--> Enter '--help' to know the number of arguments to be passed based on operation.\n";
        return 0;
    }
    if(argc<3) //check if argument count is correct.
    {
        if(argc == 2 && (strcmp(argv[1],"--help") == 0))
        {
            cout<<"HELP OPTION : \n";
            cout<<"Encoding ---> ./a.out -e <.bmp_file> <.text_file> [output file]\nDecoding ---> ./a.out -d <.bmp_file> [output file]\n";
            return 0;
        }
        else
        {
            cout<<"Insufficient number of arguments.\nEnter <.bmp_file> and <.text_file>\n";
            return 0;
        }
    }
    if(strcmp(argv[1],"-e") == 0)
    {
        if(argc == 4 || argc == 5)
        {
            Steganography stego;
            if(stego.read_and_validate_encode_args(argv) == true)
            {
                if(argc == 4)
                {
                    Encoder enc(argv[2], argv[3], "default.bmp");
                    enc.encode();
                }
                else
                {
                    Encoder enc(argv[2], argv[3], argv[4]);
                    enc.encode();
                }
            }
        }
        else
        {
            cout<<"Insufficient number of arguments for encoding.\n";
            cout<<"Encoding ---> ./a.out -e <.bmp_file> <.text_file> [output file]\n";
        }
    }
    else if(strcmp(argv[1],"-d") == 0)
    {
        if(argc == 3 || argc == 4)
        {
            Steganography stego;
            if(stego.read_and_validate_decode_args(argv) == true)
            {
                if(argc == 3)
                {
                    Decoder dec(argv[2], "default.txt");
                    dec.decode();
                }
                else
                {
                    Decoder dec(argv[2], argv[3]);
                    dec.decode();
                }
            }
        }
        else
        {
            cout<<"Insufficient number of arguments for encoding.\n";
            cout<<"Decoding ---> ./a.out -d <.bmp_file> [output file]\n";   
        }
    }
    else
    {
        cout<<"Improper argument for Encoding or decoding.\n";
        cout<<"Use '-e' for encoding or '-d' for decoding.\n";
    }
}
