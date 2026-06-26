#include "Encode.hpp"

Encode::Encode()
{
    src_image_fname = nullptr;
    secret_fname = nullptr;
    stego_image_fname = nullptr;

    fptr_src_image = nullptr;
    fptr_secret = nullptr;
    fptr_stego_image = nullptr;

    image_capacity = 0;
    size_secret_file = 0;
}

Status Encode::readAndValidateArgs(char *argv[])
{
    if (strstr(argv[2], ".bmp") != NULL)
    {
        src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if (strstr(argv[3], ".txt") != NULL)
    {
        secret_fname = argv[3];
    }
    else
    {
        return e_failure;
    }

    if (argv[4] != NULL)
    {
        stego_image_fname = argv[4];
    }
    else
    {
        stego_image_fname = (char *)"default.bmp";
    }

    return e_success;
}

Status Encode::openFiles()
{
    fptr_src_image = fopen(src_image_fname, "r");

    if (fptr_src_image == NULL)
    {
        perror("fopen");
        cout << "ERROR : Unable to open "
             << src_image_fname << endl;
        return e_failure;
    }

    fptr_secret = fopen(secret_fname, "r");

    if (fptr_secret == NULL)
    {
        perror("fopen");
        cout << "ERROR : Unable to open "
             << secret_fname << endl;
        return e_failure;
    }

    fptr_stego_image = fopen(stego_image_fname, "w");

    if (fptr_stego_image == NULL)
    {
        perror("fopen");
        cout << "ERROR : Unable to open "
             << stego_image_fname << endl;
        return e_failure;
    }

    return e_success;
}

uint Encode::getImageSizeForBmp()
{
    uint width, height;

    fseek(fptr_src_image, 18, SEEK_SET);

    fread(&width, sizeof(int), 1,
          fptr_src_image);

    fread(&height, sizeof(int), 1,
          fptr_src_image);

    cout << "width = "
         << width << endl;

    cout << "height = "
         << height << endl;

    return width * height * 3;
}

uint Encode::getFileSize(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);

    uint size = ftell(fptr);

    return size;
}

Status Encode::checkCapacity()
{
    image_capacity =
        getImageSizeForBmp();

    size_secret_file =
        getFileSize(fptr_secret);

    if (image_capacity >
        (16 + 32 + 32 + 32 +
         (size_secret_file * 8)))
    {
        return e_success;
    }

    return e_failure;
}

Status Encode::copyBmpHeader()
{
    char header[54];

    fseek(fptr_src_image,
          0,
          SEEK_SET);

    fread(header,
          54,
          1,
          fptr_src_image);

    fwrite(header,
           54,
           1,
           fptr_stego_image);

    return e_success;
}

Status Encode::encodeByteToLSB(char data,
                               char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        unsigned char bit =
            (data >> i) & 1;

        image_buffer[i] =
            (image_buffer[i] & 0xFE) | bit;
    }

    return e_success;
}

Status Encode::encodeDataToImage(char *data,
                                 int size)
{
    for (int i = 0; i < size; i++)
    {
        fread(image_data,
              8,
              1,
              fptr_src_image);

        encodeByteToLSB(data[i],
                        (char *)image_data);

        fwrite(image_data,
               8,
               1,
               fptr_stego_image);
    }

    return e_success;
}

Status Encode::encodeMagicString()
{
    encodeDataToImage(
        (char *)MAGIC_STRING,
        strlen(MAGIC_STRING));

    return e_success;
}

Status Encode::encodeSizeToLSB(int size)
{
    char buffer[32];

    fread(buffer,
          32,
          1,
          fptr_src_image);

    for (int i = 0; i < 32; i++)
    {
        buffer[i] =
            (buffer[i] & 0xFE) |
            ((size >> i) & 1);
    }

    fwrite(buffer,
           32,
           1,
           fptr_stego_image);

    return e_success;
}

Status Encode::encodeSecretFileExtnSize()
{
    encodeSizeToLSB(
        strlen(extn_secret_file));

    return e_success;
}

Status Encode::encodeSecretFileExtn()
{
    encodeDataToImage(
        extn_secret_file,
        strlen(extn_secret_file));

    return e_success;
}

Status Encode::encodeSecretFileSize()
{
    encodeSizeToLSB(
        size_secret_file);

    return e_success;
}

Status Encode::encodeSecretFileData()
{
    char buffer[size_secret_file];

    fseek(fptr_secret,
          0,
          SEEK_SET);

    fread(buffer,
          1,
          size_secret_file,
          fptr_secret);

    encodeDataToImage(
        buffer,
        size_secret_file);

    return e_success;
}

Status Encode::copyRemainingImageData()
{
    int rem_bytes =
        54 +
        image_capacity -
        ftell(fptr_src_image);

    char buffer[rem_bytes];

    fread(buffer,
          rem_bytes,
          1,
          fptr_src_image);

    fwrite(buffer,
           rem_bytes,
           1,
           fptr_stego_image);

    return e_success;
}

Status Encode::doEncoding()
{
    if (openFiles() != e_success)
    {
        cout << "ERROR : Open files failed"
             << endl;

        return e_failure;
    }

    cout << "Files opened successfully"
         << endl;

    if (checkCapacity() != e_success)
    {
        cout << "Check capacity failed"
             << endl;

        return e_failure;
    }

    cout << "Check capacity success"
         << endl;

    if (copyBmpHeader() != e_success)
    {
        return e_failure;
    }

    cout << "BMP Header copied"
         << endl;

    encodeMagicString();

    cout << "Magic string encoded"
         << endl;

    strcpy(extn_secret_file,
           strstr(secret_fname, "."));

    encodeSecretFileExtnSize();

    cout << "Extension size encoded"
         << endl;

    encodeSecretFileExtn();

    cout << "Extension encoded"
         << endl;

    encodeSecretFileSize();

    cout << "File size encoded"
         << endl;

    encodeSecretFileData();

    cout << "Secret data encoded"
         << endl;

    copyRemainingImageData();

    cout << "Remaining image copied"
         << endl;

    fclose(fptr_src_image);
    fclose(fptr_secret);
    fclose(fptr_stego_image);

    return e_success;
}