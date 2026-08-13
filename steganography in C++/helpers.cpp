#include<iostream>
#include<cstring>
#include<fstream>
#include "decoder.h"
#include "encoder.h"
#include "steganography.h"

void Encoder::get_extension()
{
    const char *ext = strrchr(secretFileName.c_str(), '.');
    if(ext != NULL)
    {
        extension = ext;
        extensionSize = extension.length();
    }
}

bool Encoder::encode_byte_to_lsb(char data, char *buffer)
{
    for(int i=0;i<8;i++)
    {
        int bit = (data>>(7-i)) & 1;
        buffer[i] = buffer[i] & 0xFE;
        buffer[i] = buffer[i] | bit;
    }
    return true;
}

bool Encoder::encode_int_to_lsb(int data,char *buffer)
{
    for(int i=0;i<32;i++)
    {
        int bit = (data>>(31-i)) & 1;
        buffer[i] = buffer[i] & 0xFE;
        buffer[i] = buffer[i] | bit;
    }
    return true;
}

bool Decoder::decode_byte_from_lsb(char *data, char *buffer)
{
    char ch=0;
    for(int i=0;i<8;i++) 
    {
        ch = (ch<<1); 
        ch = ch | (buffer[i] & 1);
    }
    *data=ch;
    return true;
}

bool Decoder::decode_int_from_lsb(int *size, char *buffer)
{
    int result=0;
    for(int i=0;i<32;i++)
    {
        result = (result<<1);
        result = result | (buffer[i] & 1);
    }
    *size=result;
    return true;
}