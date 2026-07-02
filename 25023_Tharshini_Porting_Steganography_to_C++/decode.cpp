/*NAME        : THARSHINI S
  DATE        : 27-05-2026
  DESCRIPTION : PORTING STEGANOGRAPHY TO C++ - Implemented image steganography in C++ to hide and retrieve secret data inside BMP images.*/
  
#include "types.h"
#include "common.h"
#include "decode.h"

using namespace std;

//constructor
Decode::Decode(string stego, string output) : stego_image_fname(stego), output_fname(output) //initializer list
{
    
}

//open files
Status Decode::open_decode_files()
{
    // Stego Image file
    fptr_stego_image = fopen(stego_image_fname.c_str(), "r");
    // Do Error handling
    if(fptr_stego_image==NULL)
    {
    	perror("fopen");
    	cerr<<"ERROR: Unable to open file "<<stego_image_fname<<endl;
    	return d_failure;
    }

     // output file
    fptr_output = fopen(output_fname.c_str(), "w");
    // Do Error handling
    if(fptr_output==NULL)
    {
    	perror("fopen");
    	cerr<<"ERROR: Unable to open file "<<output_fname<<endl;
        fclose(fptr_stego_image);
    	return d_failure;
    }
    return d_success;   
}

//decode magic string
Status Decode::magic_string()
{
    string magic = "";
    char buf[8];
    rewind(fptr_stego_image);
    fseek(fptr_stego_image,54,SEEK_SET); //skip bmp header
    //Read the magic string length of bytes after skipping the BMP header
    for(int i=0;i<MAGIC_STRING.length();i++)
    {
        fread(buf,8,1,fptr_stego_image);
        char ch=0;
        for(int j=0;j<8;j++)
        {
            int bit=buf[j]&0x01;
            ch = ch | (bit<<j);
        }
        magic+=ch;
    }
    
    if(magic.compare(MAGIC_STRING)==0)
    {
        cout<<"Magic string is matched"<<endl;
        return d_success;
    }
    else
    {
        cout<<"ERROR : Magic string is not matched"<<endl;
        return d_failure;
    }
}

//decode size from lsb
Status Decode::decode_size_from_lsb(FILE *fptr, long *size)
{
    *size=0;
     char buffer[32];
    fread(buffer,32,1,fptr);
     for(int i=0;i<32;i++)
     {
        int bit=buffer[i]&0x01;//extract lsb
        *size=*size | (bit<<i);//set bit in size at position i
     }
     return d_success;
}

//decode data from image
Status Decode::decode_data_from_image(FILE *fptr, string &data, int size)
{
    char buffer[8];
    data="";
    for(int i=0;i<size;i++)
    {
        fread(buffer,8,1,fptr);
        char ch=0;
        for(int j=0;j<8;j++)
        {
            int bit=buffer[j]&0x01;//extract lsb
            ch=ch | (bit<<j); //combine bits into byte
        }
        data+=ch;
    }
    return d_success;
}

//decode extension size
Status Decode::decode_secret_file_extn_size()
{
    decode_size_from_lsb(fptr_stego_image,&size_extn);
    return d_success;
}

//decode extension
Status Decode::decode_secret_file_extn()
{
    decode_data_from_image(fptr_stego_image,extn_secret_file,size_extn);
    return d_success;
}

//decode secret file size
Status Decode::decode_secret_file_size()
{
    decode_size_from_lsb(fptr_stego_image,&size_secret_file);
    return d_success;
}

//decode secret file data
Status Decode::decode_secret_file_data()
{
    string secret;
    decode_data_from_image(fptr_stego_image,secret,size_secret_file);
    fwrite(secret.c_str(),size_secret_file,1,fptr_output);
    return d_success;
}

//Decoding process
Status Decode::do_decoding()
{
    if(open_decode_files()==d_success)
    {
        cout<<"Info : Files opened successfully..."<<endl;
        if(magic_string()==d_success)
        {
            cout<<"Magic string is decoded successfully"<<endl;
            if(decode_secret_file_extn_size()==d_success)
            {
                 cout<<"Decoded secret file extension size successfully"<<endl;
                 if(decode_secret_file_extn()==d_success)
                 {
                     cout<<"Decoded secret file extension successfully"<<endl;
                     if(decode_secret_file_size()==d_success)
                     {
                         cout<<"Decoded secret file size is successfully"<<endl;
                         if(decode_secret_file_data()==d_success)
                         {
                            cout<<"Decoded secret file data successfully"<<endl;
                         }
                         else
                         {
                            cout<<"Failed to decode secret data"<<endl;
                            return d_failure;
                         }

                     }
                     else
                     {
                        cout<<"Failed to decode secret file size"<<endl;
                        return d_failure;
                     }
                 }
                 else
                 {
                    cout<<"Failed to decode secret file extension"<<endl;
                    return d_failure;
                 }
            }
            else
            {
                cout<<"Failed to decode secret file extension size"<<endl;
                return d_failure;
            }
        }
        else
        {
            cout<<"Magic string is mismatch"<<endl;
            return d_failure;

        }
     
    }
    else
    {
        cout<<"ERROR : Failed to open files"<<endl;
        return d_failure;
    }
    return d_success;
}