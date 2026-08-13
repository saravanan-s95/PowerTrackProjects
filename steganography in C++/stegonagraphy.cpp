#include<iostream>
#include<cstring>
#include<fstream>
#include "decoder.h"
#include "encoder.h"
#include "steganography.h"

bool Steganography::read_and_validate_encode_args(char *argv[])
{
    //VALIDATE .bmp extension for source file.
    char *p=strstr(argv[2],".");
    if(p!=NULL && (strcmp(p,".bmp") == 0))
    {
        p=strstr(argv[3],".");
        // printf("HI\n");
        if(p!=NULL && (strcmp(p,".txt") == 0))
        {
            if(argv[4] != NULL)
            {
                p=strstr(argv[4],".");
                if(p!=NULL && (strcmp(p,".bmp") == 0))
                {
                    // Encoder(argv[2],argv[3],argv[4]);
                    return true;
                }
                else
                {
                    cout<<"Enter filename with '.bmp' extension only.\n";
                    return false;
                }  
            }
            else
            {
                // Encoder(argv[2],argv[3],"default.bmp");
                return true;
            }
        }
        else
        {
            cout<<"Enter filename with '.txt' extension only.\n"; 
            return false;
        }
    }
    else
    {
        cout<<"Enter filename with '.bmp' extension only.\n";
        return false;
    }
    return true;
}

bool Steganography::read_and_validate_decode_args(char *argv[])
{
    char *p = strstr(argv[2],".");
    if(p!=NULL && (strcmp(p,".bmp") == 0))
    {
        if(argv[3] != NULL)
        {
            p = strstr(argv[3],".");
            if(p!=NULL && (strcmp(p,".txt") == 0))
            {
                return true;
            }
            else
            {
                cout<<"Enter filename with '.txt' extension only.\n"; 
                return false;
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        cout<<"Enter filename with '.bmp' extension only.\n";
        return false;
    }
}