#include "decode.h"
#include "types.h"
#include "common.h"

Status Decoder :: open_files_decode()
{
    // Encoded output Image file
    fptr_encoded_output_image.open(encoded_output_fname,ios::binary);
    // Do Error handling
    if(!fptr_encoded_output_image)
    {
        cout << "ERROR: Unable to open file " << encoded_output_fname << endl;
        return e_failure;
    }
    return e_success;
}

Status Decoder :: read_and_validate_decode_args(char* argv[])
{
    //Check input bmp file
    if(argv[2] != NULL)
    {
        //Check extension
        string bmp_file = argv[2];
        int dot_position = bmp_file.find('.');
        if(dot_position != string::npos && bmp_file.substr(dot_position) == ".bmp")
        {
            //Store file name
            encoded_output_fname = argv[2];
            cout << "Encoded file name added successfully\n";
        }
        else
        {
            //Return failure
            return e_failure;
        }
    }
    else
    {
        //File not found
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        //Store without extension
        string secret_extn = argv[3];
        int dot_position = secret_extn.find('.');
        if(dot_position != string::npos)
        {
            //Copy user entered name
            secret_fname_decode = secret_extn.substr(0,dot_position);
        }
        else
        {
            secret_fname_decode = secret_extn;
        }
    }
    else
    {
        //Give default file name without extension
        secret_fname_decode = "decode";
    }
    return e_success;
}

Status Decoder :: skip_bmp_header()
{
    //Skip 54 bytes of bmp header
    fptr_encoded_output_image.seekg(54,ios::beg);
    //Check offset position
    long var = fptr_encoded_output_image.tellg();
    if(var == 54)
    {
        //If it is in 54 position
        return e_success;
    }
    else
    {
        //Failure
        return e_failure;
    }
}

Status Decoder :: decode_byte_from_lsb(char* data,char* image_buffer)
{
    *data = 0;
    for(int i=0;i<8;i++)
    {
        //Take 8 bytes lsb and store it in data by left shifting
        *data = *data | ((image_buffer[i] & 1) << i);
    }

    return e_success;
}

Status Decoder :: decode_magic_string(const string &magic_string)
{
    for(int i=0;i<magic_string.size();i++)
    {
        //Take char and pass its address to decode lsb bytes function and then compare with each character of MAGIC STRING
        char ch;
        //Read 8 bytes of data from encoded output file(bmp) and store into image_data
        fptr_encoded_output_image.read(image_data_decode,8);
        //Decode 8 bytes of data into 1 byte
        decode_byte_from_lsb(&ch,image_data_decode);
        if(ch != magic_string[i])
        {
            return e_failure;
        }
    }
    return e_success;
}

Status Decoder :: decode_secret_file_extn_size()
{
    //Implicit conversion from long to char
    char* ptr = (char*)&size_extn_file_decode;

    for(int i=0;i<4;i++)
    {
        //Take char and pass its address to decode lsb bytes function and store the result in size_extn_file
        char ch;
        //Read 8 bytes of data from encoded output file(bmp) and store into image_data
        fptr_encoded_output_image.read(image_data_decode,8);
        //Decode 8 bytes of data into 1 byte
        decode_byte_from_lsb(&ch,image_data_decode);

        ptr[i] = ch;
    }

    if(size_extn_file_decode > 16)      // no real extension is this long
    {
        return e_failure;
    }
    return e_success;
}

Status Decoder :: decode_secret_file_extn()
{
    extn_secret_file_decode.resize(size_extn_file_decode);

    for(int i=0;i<size_extn_file_decode;i++)
    {
        //Read 8 bytes of data from encoded output file(bmp) and store into image_data
        fptr_encoded_output_image.read(image_data_decode,8);
        //Decode 8 bytes of data into 1 byte
        decode_byte_from_lsb(&extn_secret_file_decode[i],image_data_decode);
    }
    //Add the extension to the output file name 
    secret_fname_decode += extn_secret_file_decode;
    return e_success;
}

Status Decoder :: open_files_decode_file()
{
    fptr_secret_decode.open(secret_fname_decode,ios::binary);
    // Do Error handling
    if(!fptr_secret_decode)
    {
    	cout <<  "ERROR: Unable to open file " << secret_fname_decode << endl;
    	return e_failure;
    }
    return e_success;
}

Status Decoder :: decode_secret_file_size()
{
    //Implicit conversion from long to char
    char* ptr = (char*)&file_size_decode;

    for(int i=0;i<4;i++)
    {
        //Take char and pass its address to decode lsb bytes function and store the result in size_extn_file
        char ch;
        //Read 8 bytes of data from encoded output file(bmp) and store into image_data
        fptr_encoded_output_image.read(image_data_decode,8);
        //Decode 8 bytes of data into 1 byte
        decode_byte_from_lsb(&ch,image_data_decode);

        ptr[i] = ch;
    }
    return e_success;
}

Status Decoder :: decode_secret_file_data()
{
    for(int i=0;i<file_size_decode;i++)
    {
        char ch;
        //Read 8 bytes of data from encoded output file(bmp) and store into image_data
        fptr_encoded_output_image.read(image_data_decode,8);
        //Decode 8 bytes of data into 1 byte
        decode_byte_from_lsb(&ch,image_data_decode);

        fptr_secret_decode.write(&ch,1);
    }
    return e_success;
}

void Decoder :: close_files()
{
    fptr_encoded_output_image.close();
    fptr_secret_decode.close();
}

Status Decoder :: do_decoding()
{
    //Open encoded .bmp file in read mode
    if(open_files_decode() == e_failure)
    {
        //Failure
        cout << "Encoded file not opened failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Encoded file opened successfully\n";
    }

    //Skip 54 bytes of bmp header
    if(skip_bmp_header() == e_failure)
    {
        //Failure
        cout << "Skipped bmp header failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Skipped bmp header successfully\n";
    }

    //Decode the magic string from the encoded file 
    if(decode_magic_string(MAGIC_STRING) == e_failure)
    {
        //Failure
        cout << "Magic string decode failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Magic string decoded successfully\n";
    }

    //Decode secret file extension size from the encoded file
    if(decode_secret_file_extn_size() == e_failure)
    {
        //Failure
        cout << "Secret file extension size decode failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Secret file extension size decoded successfully\n";
    }

    //Decode secret file extension from the encoded file
    if(decode_secret_file_extn() == e_failure)
    {
        //Failure
        cout << "Secret file extension failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Secret file extension decoded successfully\n";
    }

    //Open the output file
    if(open_files_decode_file() == e_failure)
    {
        //Failure
        cout << "Output file not opened failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Output file opened successfully\n";
    }

    //Decode secret file size from the encoded file
    if(decode_secret_file_size() == e_failure)
    {
        //Failure
        cout << "Secret file size failed\n";
        return e_failure;
    }
    else
    {
        //Success
        cout << "Secret file size decoded successfully\n";
    }

    //Decode the actual secret data from the encoded file and store it in output file
    if(decode_secret_file_data() == e_failure)
    {
        //Failure
        cout << "Secret file data failed\n";
        close_files();
        return e_failure;
    }
    else
    {
        //Success
        cout << "Secret file data decoded successfully\n";
        close_files();
        return e_success;
    }
}
