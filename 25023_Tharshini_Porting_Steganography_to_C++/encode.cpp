/*NAME        : THARSHINI S
  DATE        : 27-05-2026
  DESCRIPTION : PORTING STEGANOGRAPHY TO C++ - Implemented image steganography in C++ to hide and retrieve secret data inside BMP images.*/
  
#include "types.h"
#include "common.h"
#include "encode.h"

//constructor
Encode::Encode(string src, string secret, string stego)
{
    src_image_fname=src;
    secret_fname=secret;
    stego_image_fname=stego;
}

//get image size
uint Encode::get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    cout<<"width = "<<width<<endl;

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    cout<<"height = "<<height<<endl;

    // Return image capacity
    return width * height * 3;
}

//open files
Status Encode::open_files()
{
    // Src Image file
    fptr_src_image = fopen(src_image_fname.c_str(), "r");
    // Do Error handling
    if(fptr_src_image == NULL)
    {
    	perror("fopen");
    	cerr<<"ERROR: Unable to open file "<<src_image_fname<<endl;

    	return e_failure;
    }

    // Secret file
    fptr_secret = fopen(secret_fname.c_str(), "r");
    // Do Error handling
    if(fptr_secret == NULL)
    {
    	perror("fopen");
    	cerr<<"ERROR: Unable to open file "<<secret_fname<<endl;

    	return e_failure;
    }

    // Stego Image file
    fptr_stego_image = fopen(stego_image_fname.c_str(), "w");
    // Do Error handling
    if(fptr_stego_image == NULL)
    {
    	perror("fopen");
    	cerr<<"ERROR: Unable to open file "<<stego_image_fname<<endl;

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

//get file size
uint Encode::get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size=ftell(fptr);
    rewind(fptr);    
    return size;
}

//check capacity
Status Encode::check_capacity()
{
  image_capacity = get_image_size_for_bmp(fptr_src_image);
  size_secret_file = get_file_size(fptr_secret);
  if(image_capacity > 16+32+32+32+ size_secret_file*8)
  {
    return e_success;
  }
  else
  {
    return e_failure;
  }
}

//Copy the first 54 bytes of BMP header from source image to stego image
Status Encode::copy_bmp_header()
{
    char bmp_header[54];
    rewind(fptr_src_image);
    fread(bmp_header,54,1,fptr_src_image);
    fwrite(bmp_header,54,1,fptr_stego_image);
    return e_success;
}

//Encodes a single byte into the LSBs of 8 image bytes
Status Encode::encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i] &0xFE)|((data >>i) & 0x01);
    }
    return e_success;    
}

//Encodes a string of data into the image using LSB technique
Status Encode::encode_data_to_image(string data, int size)
{
    for(int i=0;i<size;i++)
    {
    //read 8 bytes from src file
    fread(image_data,8,1,fptr_src_image);
    //to perform encoding pass 1 char and 8 bytes of data
    encode_byte_to_lsb(data[i],image_data);
    //write encoded 8 bytes to output file
    fwrite(image_data,8,1,fptr_stego_image);
    }
    return e_success;
}

//Encodes predefined magic string to identify stego image
Status Encode::encode_magic_string(string magic_string)
{
   encode_data_to_image(magic_string,magic_string.length());
   return e_success;
}

//Encode a 32-bit integer into LSBs of 32 image bytes
Status Encode::encode_size_to_lsb(int size)
{
    char str[32];
    fread(str,32,1,fptr_src_image);
    //to encode int-32bits only in lsb
    for(int i=0;i<32;i++)
    {
        str[i]=(str[i] &0xFE)|((size >>i) & 0x01);//0 1 2 3 4 5 6 7 
    }
    fwrite(str,32,1,fptr_stego_image);
    return e_success;
}

//Encode the length of the secret file extension
Status Encode::encode_secret_file_extn_size(int size)
{
    encode_size_to_lsb(size);
    return e_success;
}

//Encode the secret file extension
Status Encode::encode_secret_file_extn(string file_extn)
{
encode_data_to_image(file_extn,file_extn.length());
return e_success;
}

//Encode the size of the secret file
Status Encode::encode_secret_file_size(long file_size)
{
    encode_size_to_lsb(file_size);
    return e_success;
}

//Encode the actual content of the secret file
Status Encode::encode_secret_file_data()
{
    char data[size_secret_file];
    rewind(fptr_secret);
    fread(data,1,size_secret_file,fptr_secret);
    encode_data_to_image(data,size_secret_file);
    return e_success;
}

//Copy the remaining image data after encoding
Status Encode::copy_remaining_img_data()
{
    int size;
    size=image_capacity + 54 - ftell(fptr_src_image);
    char remaining_data[size];
    fread(remaining_data,size,1,fptr_src_image);
    fwrite(remaining_data,size,1,fptr_stego_image);
    return e_success;
}

//encoding process is performed here
Status Encode::do_encoding()
{
    if(open_files()==e_success)
    {
        cout<<"Info : Files opened successfully..."<<endl;
        if(check_capacity()==e_success)
        {
            cout<<"Secret data can fit into the given image file"<<endl;
            if(copy_bmp_header()==e_success)
            {
                cout<<"Copied BMP header successfully"<<endl;
                if(encode_magic_string(MAGIC_STRING)==e_success)
                {
                    cout<<"Encoded magic string successfully"<<endl;
                    extn_secret_file=secret_fname.substr(secret_fname.find("."));
                    cout<<extn_secret_file<<endl;
                    if(encode_secret_file_extn_size(extn_secret_file.length())==e_success)
                    {
                        cout<<"Encoded secret file extension size successfully"<<endl;
                        if(encode_secret_file_extn(extn_secret_file)==e_success)
                        {
                            cout<<"Encoded secret file extension successfully"<<endl;
                            if(encode_secret_file_size(size_secret_file)==e_success)
                            {
                                cout<<"Encoded secret file size successfully"<<endl;
                                if(encode_secret_file_data()==e_success)
                                {
                                    cout<<"Encoded secret file data successfully"<<endl;
                                    if(copy_remaining_img_data()==e_success)
                                    {
                                        cout<<"Copied remaining data successfully"<<endl;
                                    }
                                    else
                                    {
                                        cout<<"Failed to copy remaining data"<<endl;
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    cout<<"Failed to encode secret file data"<<endl;
                                    return e_failure;
                                }
                            }
                            else
                            {
                                cout<<"Failed to encode secret file size"<<endl;
                                return e_failure;
                            }
                        }
                        else
                        {
                            cout<<"Failed to encode secret file extension"<<endl;
                            return e_failure;
                        }
                    }
                    else
                    {
                        cout<<"Failed to encode secret file extension size"<<endl;
                        return e_failure;
                    }
                }
                else
                {
                    cout<<"Failed to encode magic string"<<endl;
                    return e_failure;
                }
            }
            else
            {
                cout<<"Failed to copy BMP header"<<endl;
                return e_failure;
            }
        }
        else
        {
            cout<<"cannot fit Secret data in the given image file..."<<endl;
        }
    }
    else
    {
        cout<<"ERROR : Failed to open files"<<endl;
        return e_failure;
    }
    return e_success;//all steps are successfull
}
