#include<iostream>
#include<cstring>
#include<fstream>
#include "decoder.h"
#include "encoder.h"
#include "steganography.h"

using namespace std;

bool Encoder::encode()
{
    get_extension();
    cout<<"Encode function reached...\n";
    if(openFiles() == true)
    {
        cout<<"All files opened successfully.......\n";
    }
    else
    {
        cout<<"Error: Opening files.\n";
        return false;   
    }

    if(checkCapacity() == true)
    {
        cout<<"file capacity check done....\n";
    }
    else
    {
        cout<<"Error: File capacity not enough.\n";
        return false;
    }

    if(copyHeader() == true)
    {
        cout<<"Header copied successfully....\n";
    }
    else
    {
        cout<<"Error: Header copying failed.\n";
        return false;
    }

    if(encodeMagicString() == true)
    {
        cout<<"Encoded magic string successfully....\n";
    }
    else
    {
        cout<<"Error: Magic string encoding failed.\n";
        return false;
    }

    if(encodeExtensionSize() == true)
    {
        cout<<"Encoded Extension Size successfully....\n";
    }
    else
    {
        cout<<"Error: extension size encoding failed.\n";
        return false;
    }

    if(encodeExtension() == true)
    {
        cout<<"Encoded Extension successfully....\n";
    }
    else
    {
        cout<<"Error: extension encoding failed.\n";
        return false;
    }

    if(encodeSecretSize() == true)
    {
        cout<<"Encoded secret size successfully....\n";
    }
    else
    {
        cout<<"Error: secret size encoding failed.\n";
        return false;
    }

    if(encodeSecretData() == true)
    {
        cout<<"Encoded secret data successfully....\n";
    }
    else
    {
        cout<<"Error: secret data encoding failed.\n";
        return false;
    }

    if(copyRemainingData() == true)
    {
        cout<<"Remaining data copied successfully....\n";
        cout<<"ENCODING PROCESS SUCCESSFULL.\n";
        cout<<"ENCODING DONE TO "<<outputImageName<<endl;
        imageFile.close();
        secretFile.close();
        outputImage.close();
        return true;
    }
    else
    {
        cout<<"Error: copying of remaining data failed.\n";
        return false;
    }
    // return true;    
}

bool Encoder::openFiles()
{
    imageFile.open(inputImageName, ios::in | ios::binary);
    if(!imageFile)
    {
        cout << "Unable to open source image file.\n";
        return false;
    }

    secretFile.open(secretFileName, ios::in);
    if(!secretFile)
    {
        cout << "Unable to open secret file.\n";
        return false;
    }

    outputImage.open(outputImageName, ios::out | ios::binary);
    if(!outputImage)
    {
        cout << "Unable to create output image.\n";
        return false;
    }
    return true;
}

bool Encoder::checkCapacity()
{
    imageFile.seekg(0, ios::end);
    imageCapacity = imageFile.tellg();
    imageFile.seekg(0, ios::beg);

    secretFile.seekg(0, ios::end);
    secretSize = secretFile.tellg();
    secretFile.seekg(0, ios::beg);

    int usableCapacity = imageCapacity - 54;

    int requiredBytes = strlen(MAGIC_STRING) + sizeof(int) + extensionSize + sizeof(int) + secretSize;

    int requiredCapacity = requiredBytes * 8;

    if(usableCapacity >= requiredCapacity)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Encoder::copyHeader()
{
    // cout << "Copying header...\n";
    // return true;
    char buffer[54];
    imageFile.read(buffer,54);
    outputImage.write(buffer,54);
    return true;
}

bool Encoder::encodeMagicString()
{
    // cout << "encoding Magic String...\n";
    // return true;
    char magic_string[sizeof(MAGIC_STRING)];
    strcpy(magic_string,MAGIC_STRING);
    // cout<<magic_string<<endl;

    char buffer[8];
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        imageFile.read(buffer,8);
        encode_byte_to_lsb(magic_string[i],buffer);
        outputImage.write(buffer,8);
    }
    return true;
}

bool Encoder::encodeExtensionSize()
{
    // cout << "encoding Extension...\n";
    // return true;
    // int extensionSize = 4;
    // cout<<extensionSize<<endl;
    char buffer[32];
    imageFile.read(buffer,32);
    encode_int_to_lsb(extensionSize,buffer);
    outputImage.write(buffer,32);
    return true;
}

bool Encoder::encodeExtension()
{
    // cout << "encoding Extension...\n";
    // cout<<extension<<endl;
    char buffer[8];
    for(int i=0;i<extensionSize;i++)
    {
        imageFile.read(buffer,8);
        encode_byte_to_lsb(extension[i],buffer);
        outputImage.write(buffer,8);
    }
    return true;
    // char 
}

bool Encoder::encodeSecretSize()
{
    // cout << "encoding Secret Size...\n";
    char buffer[32];
    imageFile.read(buffer,32);
    encode_int_to_lsb(secretSize,buffer);
    outputImage.write(buffer,32);
    return true;
}

bool Encoder::encodeSecretData()
{
    // cout << "encoding Secret Data...\n";
    char buffer[8];
    char secret_data_buffer[secretSize];
    secretFile.seekg(0,ios::beg);
    secretFile.read(secret_data_buffer,secretSize);
    for(int i=0;i<secretSize;i++)
    {
        imageFile.read(buffer,8);
        encode_byte_to_lsb(secret_data_buffer[i],buffer);
        outputImage.write(buffer,8);
    }
    return true;
}

bool Encoder::copyRemainingData()
{
    // cout << "copying Remaining Data...\n";
    char buffer;
    while(imageFile.read(&buffer,1))
    {
        outputImage.write(&buffer,1);
    }
    return true;
}