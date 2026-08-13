#include<iostream>
#include<cstring>
#include<fstream>
#include "decoder.h"
#include "encoder.h"
#include "steganography.h"

using namespace std;

bool Decoder::decode()
{
    if(openFiles() == true)
    {
        cout<<"Files opened successfully.\n";
    }
    else
    {
        cout<<"Error: Opening files.\n";
        return false;
    }

    if(decodeMagicString() == true)
    {
        cout<<"Magic string decoded successfully.\n";
    }
    else
    {
        cout<<"Error: Decoding magic string.\n";
        return false;
    }

    if(decodeExtensionSize() == true)
    {
        cout<<"Extension size decoded successfully.\n";
    }
    else
    {
        cout<<"Error: Decoding extension size.\n";
        return false;
    }

    if(decodeExtension() == true)
    {
        cout<<"Extension decoded successfully.\n";
    }
    else
    {
        cout<<"Error: Decoding extension.\n";
        return false;
    }

    if(decodeSecretSize() == true)
    {
        cout<<"Secret data size decoded successfully.\n";
    }
    else
    {
        cout<<"Error: Decoding secret data size.\n";
        return false;
    }

    if(decodeSecretData() == true)
    {
        cout<<"Secret data decoded successfully.\n";
        cout<<"Decoded secret data to '"<<outputFileName<<"'"<<endl;
        imageFile.close();
        outputFile.close();
        return true;
    }
    else
    {
        cout<<"Error: Decoding Secret data.\n";
        return false;
    }
    return true;
}

bool Decoder::openFiles()
{
    imageFile.open(stegoImageName,ios::in | ios::binary);
    if(!imageFile)
    {
        cout<<"Unable to open source image file.\n";
        return false;
    }
    outputFile.open(outputFileName,ios::out);
    if(!outputFile)
    {
        cout<<"Unable to open "<<outputFileName<<"file.\n";
        return false;
    }
    return true;
}

bool Decoder::decodeMagicString()
{
    imageFile.seekg(54,ios::beg);
    // imageFile.tellg();
    char buffer[8];
    char magic_string[4];
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        imageFile.read(buffer,8);
        decode_byte_from_lsb(&magic_string[i],buffer);
    }
    // cout<<magic_string<<endl;
    if(strcmp(MAGIC_STRING,magic_string) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Decoder::decodeExtensionSize()
{
    char buffer[32];
    imageFile.read(buffer,32);
    decode_int_from_lsb(&extension_size,buffer);
    // cout<<extension_size<<endl;
    if(extension_size == sizeof(int))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Decoder::decodeExtension()
{
    char buffer[8];
    char extension_buffer[extension_size];
    for(int i=0;i<extension_size;i++)
    {
        imageFile.read(buffer,8);
        decode_byte_from_lsb(&extension_buffer[i],buffer);
    }
    // cout<<extension_buffer<<endl;
    extension = extension_buffer;
    // cout<<extension<<endl;
    return true;
}

bool Decoder::decodeSecretSize()
{
    char buffer[32];
    imageFile.read(buffer,32);
    decode_int_from_lsb(&secretSize,buffer);
    // cout<<secretSize<<endl;
    return true;
}

bool Decoder::decodeSecretData()
{
    char buffer[8];
    char secret_data[secretSize];
    for(int i=0;i<secretSize;i++)
    {
        imageFile.read(buffer,8);
        decode_byte_from_lsb(&secret_data[i],buffer);
    }
    outputFile.write(secret_data,secretSize);
    return true;
}