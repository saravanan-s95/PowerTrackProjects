#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint Encoder :: get_image_size_for_bmp()
{
    uint width,height;
    // Seek to 18th byte
    fptr_src_image.seekg(18,ios::beg);

    // Read the width (an int)
    fptr_src_image.read((char*)&width,4);
    cout << "width = " << width << endl;

    // Read the height (an int)
    fptr_src_image.read((char*)&height,4);
    cout << "height = " << height << endl;

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File stream objects for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: File stream objects for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status Encoder :: open_files()
{
    // Src Image file
    fptr_src_image.open(src_image_fname,ios::binary);
    // Do Error handling
    if(!fptr_src_image)
    {
        cout << "ERROR: Unable to open file " << src_image_fname << endl;
        return e_failure;
    }

    // Secret file
    fptr_secret.open(secret_fname,ios::binary);
    // Do Error handling
    if(!fptr_secret)
    {
        cout << "ERROR: Unable to open file " << secret_fname << endl;
        return e_failure;
    }

    // Stego Image file
    fptr_stego_image.open(stego_image_fname,ios::binary);
    // Do Error handling
    if(!fptr_stego_image)
    {
        cout << "ERROR: Unable to open file " << stego_image_fname << endl;
        return e_failure;
    }
    
    // No failure return e_success
    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    //Check encode or decode
    if(string(argv[1]) == "-e")
    {
        //Encode
        return e_encode;
    }
    else if(string(argv[1]) == "-d")
    {
        //Decode
        return e_decode;
    }
    else
    {
        //Neither 
        return e_unsupported;
    }
}

Status Encoder :: read_and_validate_encode_args(char *argv[])
{
    //Check input bmp file
    if(argv[2] != NULL)
    {
        //Check extension
        string bmp_filename = argv[2];
        int dot_position = bmp_filename.find('.');
        if(dot_position != string::npos && bmp_filename.substr(dot_position) == ".bmp")
        {
            //Store file name
            src_image_fname = argv[2];
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

    //Check secret data file
    if(argv[3] != NULL)
    {
        secret_fname = argv[3];
        //Store secret data file extension only
        int dot_position = secret_fname.find('.');
        if(dot_position != string::npos)
        {
            extn_secret_file = secret_fname.substr(dot_position);
        }
        else
        {
            //Dot not found return failure
            return e_failure;
        }
    }
    else
    {
        //File not found
        return e_failure;
    }

    //Check output bmp file
    if(argv[4] != NULL)
    {
        //Check extension
        string stego_filename = argv[4];
        int dot_position = stego_filename.find('.');
        if(dot_position != string::npos && stego_filename.substr(dot_position) == ".bmp")
        {
            //Store file name
            stego_image_fname = argv[4];
        }
        else
        {
            //Return failure
            return e_failure;
        }
    }
    else
    {
        //Give default output file name if user not given
        stego_image_fname = "encode_output_fname.bmp";
    }
    return e_success;
}

uint Encoder :: get_file_size()
{
    //Get file size
    uint size;
    //Move offset to the end
    fptr_secret.seekg(0,ios::end);
    //Store the value in unsigned int
    size = fptr_secret.tellg();
    //Move the offset back to start of the file
    fptr_secret.seekg(0,ios::beg);

    return size;
}

Status Encoder :: check_capacity()
{
    /*If total encode data < RGB of bmp file
    proceed
    else
    STOP*/
    //1.Size of RGB data
    //2.Size of data to be encoded
    //total = magic string + extension size + extension + file size + data
    size_secret_file = get_file_size();
    uint total = (string(MAGIC_STRING).size()*8) + 32 + (extn_secret_file.size()*8) + 32 + (size_secret_file*8);
    image_capacity = get_image_size_for_bmp();

    if(total <= image_capacity)
    {
        //Proceed
        return e_success;
    }
    else
    {
        //Total > RGB
        return e_failure;
    }
}

Status Encoder :: copy_bmp_header()
{
    fptr_src_image.seekg(0,ios::beg);
    char header[54];

    // Read first 54 bytes from source
    fptr_src_image.read(header,54);

    // Write first 54 bytes to destination
    fptr_stego_image.write(header,54);
    return e_success;
}

Status Encoder :: encode_byte_to_lsb(char data,char* image_buffer)
{
    //Encoding
    //1.Take each bit from data and store into image_buffer's each byte's lsb
    for(int i=0;i<8;i++)
    {
        image_buffer[i] = image_buffer[i] & ~(1) | ((data >> i) & 1);
        //Repeat for 8 times
    }
    return e_success;
}

Status Encoder :: encode_magic_string(const string &magic_string)
{
    for(int i=0;i<magic_string.size();i++)
    {
        //Read 8 bytes of data from src file(bmp) and store into image_data
        fptr_src_image.read(image_data,8);
        //Encode 1 byte of data into 8 bytes lsb
        encode_byte_to_lsb(magic_string[i],image_data);
        //Write 8 bytes of encoded data(image_data) into destination file(stego_image)
        fptr_stego_image.write(image_data,8);
    }
    return e_success;
}

Status Encoder :: encode_secret_file_extn_size()
{
    uint extn_size = extn_secret_file.size();
    char* ptr = (char*)&extn_size;

    for(int i=0;i<4;i++)
    {
        //Read 8 bytes of data from src file(bmp) and store into image_data
        fptr_src_image.read(image_data,8);
        //Encode 1 byte of data into 8 bytes lsb
        encode_byte_to_lsb(ptr[i],image_data);
        //Write 8 bytes of encoded data(image_data) into destination file(stego_image)
        fptr_stego_image.write(image_data,8);
    }
    return e_success;
}

Status Encoder :: encode_secret_file_extn()
{
    for(int i=0;i<extn_secret_file.size();i++)
    {
        //Read 8 bytes of data from src file(bmp) and store into image_data
        fptr_src_image.read(image_data,8);
        //Encode 1 byte of data into 8 bytes lsb
        encode_byte_to_lsb(extn_secret_file[i],image_data);
        //Write 8 bytes of encoded data(image_data) into destination file(stego_image)
        fptr_stego_image.write(image_data,8);
    }
    return e_success;
}

Status Encoder :: encode_secret_file_size()
{
    char* ptr = (char*)&size_secret_file;

    for(int i=0;i<4;i++)
    {
        //Read 8 bytes of data from src file(bmp) and store into image_data
        fptr_src_image.read(image_data,8);
        //Encode 1 byte of data into 8 bytes lsb
        encode_byte_to_lsb(ptr[i],image_data);
        //Write 8 bytes of encoded data(image_data) into destination file(stego_image)
        fptr_stego_image.write(image_data,8);
    }
    return e_success;
}

Status Encoder :: encode_secret_file_data()
{
    for(int i=0;i<size_secret_file;i++)
    {
        //Read 8 bytes of data from src file(bmp) and store into image_data
        fptr_src_image.read(image_data,8);
        //Read 1 byte of secret_file data and store it into secret_data structure member
        fptr_secret.read(&secret_data,1);
        //Encode 1 byte of data into 8 bytes lsb
        encode_byte_to_lsb(secret_data,image_data);
        //Write 8 bytes of encoded data(image_data) into destination file(stego_image)
        fptr_stego_image.write(image_data,8);
    }
    return e_success;
}

Status Encoder :: copy_remaining_img_data()
{
    // Copy remaining bytes till EOF
    if(fptr_src_image.peek() != EOF)
    {
        fptr_stego_image << fptr_src_image.rdbuf();
    }
    return fptr_stego_image ? e_success : e_failure;
}

void Encoder :: close_files()
{
    fptr_src_image.close();
    fptr_secret.close();
    fptr_stego_image.close();
} 

Status Encoder :: do_encoding()
{
    //Open files
    if(open_files() == e_failure)
    {
        //File not found
        cout << "Files not opened correctly\n";
        return e_failure;
    }
    else
    {
        //Print Success
        cout << "Open files function successful\n";
    }

    //Check total capacity of secret file and RGB data
    if(check_capacity() == e_failure)
    {
        //Total > RGB
        cout << "Total capacity is greater than RGB data\n";
        close_files();
        return e_failure;
    }
    else
    {
        //Print Success
        cout << "Check capacity is successful\n";
    }

    //Copy bmp header from source to destination
    if(copy_bmp_header() == e_failure)
    {
        //Failure
        cout << "Copy bmp header function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Copy bmp header function is successful\n";
    }

    //Encode the magic string to the file
    if(encode_magic_string(MAGIC_STRING) == e_failure)
    {
        //Failure
        cout << "Encode magic string function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Encode magic string function is successful\n";
    }

    //Encode secret file extension file size to the file
    if(encode_secret_file_extn_size() == e_failure)
    {
        //Failure
        cout << "Encode secret file extn size function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Encode secret file extn size function is successful\n";
    }

    //Encode secret file extension to the file
    if(encode_secret_file_extn() == e_failure)
    {
        //Failure
        cout << "Encode secret file extn function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Encode secret file extn function is successful\n";
    }

    //Encode the secret file size to the file
    if(encode_secret_file_size() == e_failure)
    {
        //Failure
        cout << "Encode secret file size function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Encode secret file size function is successful\n";
    }

    //Encode the actual secret file data to the file
    if(encode_secret_file_data() == e_failure)
    {
        //Failure
        cout << "Encode secret file data function failed\n";
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Encode secret file data function is successful\n";
    }

    //Copy the rest of the bytes of the image file to the encoded file
    if(copy_remaining_img_data() == e_failure)
    {
        //Failure
        cout << "Copy remaining img data function failed\n";
        close_files();
        return e_failure;
    }
    else
    {
        //Print success
        cout << "Copy remaining img data function is successful\n";
        close_files();
        return e_success;
    }
}
