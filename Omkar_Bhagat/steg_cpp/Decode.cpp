#include "Decode.hpp"

Decode::Decode()
{
    stego_image_fname = nullptr;
    output_fname = nullptr;

    fptr_stego_image = nullptr;
    fptr_output_file = nullptr;

    extn_size = 0;
    size_secret_file = 0;
}

Status Decode::readAndValidateArgs(char *argv[])
{
    if (argv[2] == NULL || argv[3] == NULL)
    {
        cout << "ERROR : Missing arguments" << endl;
        return e_failure;
    }

    if (strstr(argv[2], ".bmp") == NULL)
    {
        cout << "ERROR : Input file should be bmp"
             << endl;
        return e_failure;
    }

    if (strstr(argv[3], ".txt") == NULL)
    {
        cout << "ERROR : Output file should be txt"
             << endl;
        return e_failure;
    }

    stego_image_fname = argv[2];
    output_fname = argv[3];

    return e_success;
}

Status Decode::openFiles()
{
    fptr_stego_image =
        fopen(stego_image_fname, "r");

    if (fptr_stego_image == NULL)
    {
        perror("fopen");

        cout << "ERROR : Unable to open "
             << stego_image_fname
             << endl;

        return e_failure;
    }

    fptr_output_file =
        fopen(output_fname, "w");

    if (fptr_output_file == NULL)
    {
        perror("fopen");

        cout << "ERROR : Unable to open "
             << output_fname
             << endl;

        fclose(fptr_stego_image);

        return e_failure;
    }

    return e_success;
}

Status Decode::decodeByteFromLSB(
    unsigned char *image_buffer,
    unsigned char *data)
{
    unsigned char ch = 0;

    for (int i = 0; i < 8; i++)
    {
        unsigned char bit =
            image_buffer[i] & 0x01;

        ch |= (bit << i);
    }

    *data = ch;

    return e_success;
}

Status Decode::decodeDataFromImage(
    char *data,
    int size)
{
    for (int i = 0; i < size; i++)
    {
        if (fread(image_data,
                  8,
                  1,
                  fptr_stego_image) != 1)
        {
            return e_failure;
        }

        unsigned char ch;

        decodeByteFromLSB(
            image_data,
            &ch);

        data[i] = ch;
    }

    return e_success;
}

Status Decode::decodeMagicString()
{
    int len = strlen(MAGIC_STRING);

    char buffer[50];

    if (decodeDataFromImage(
            buffer,
            len) != e_success)
    {
        return e_failure;
    }

    buffer[len] = '\0';

    if (strcmp(buffer,
               MAGIC_STRING) != 0)
    {
        return e_failure;
    }

    return e_success;
}

Status Decode::decodeSecretFileExtnSize()
{
    long size = 0;

    for (int i = 0; i < 32; i++)
    {
        int ch =
            fgetc(fptr_stego_image);

        if (ch == EOF)
        {
            return e_failure;
        }

        unsigned char bit =
            ch & 0x01;

        size |=
            ((long)bit << i);
    }

    extn_size = size;

    return e_success;
}

Status Decode::decodeSecretFileExtn()
{
    if (extn_size <= 0 ||
        extn_size >= 10)
    {
        return e_failure;
    }

    for (int i = 0;
         i < extn_size;
         i++)
    {
        unsigned char ch = 0;

        for (int j = 0;
             j < 8;
             j++)
        {
            int val =
                fgetc(fptr_stego_image);

            if (val == EOF)
            {
                return e_failure;
            }

            unsigned char bit =
                val & 0x01;

            ch |= (bit << j);
        }

        extn_secret_file[i] = ch;
    }

    extn_secret_file[extn_size] = '\0';

    return e_success;
}

Status Decode::decodeSecretFileSize()
{
    long size = 0;

    for (int i = 0; i < 32; i++)
    {
        int ch =
            fgetc(fptr_stego_image);

        if (ch == EOF)
        {
            return e_failure;
        }

        unsigned char bit =
            ch & 0x01;

        size |=
            ((long)bit << i);
    }

    size_secret_file = size;

    return e_success;
}

Status Decode::decodeSecretFileData()
{
    if (size_secret_file <= 0)
    {
        return e_failure;
    }

    for (long i = 0;
         i < size_secret_file;
         i++)
    {
        unsigned char ch = 0;

        for (int j = 0;
             j < 8;
             j++)
        {
            int val =
                fgetc(fptr_stego_image);

            if (val == EOF)
            {
                return e_failure;
            }

            unsigned char bit =
                val & 0x01;

            ch |= (bit << j);
        }

        fputc(ch,
              fptr_output_file);
    }

    return e_success;
}

Status Decode::doDecoding()
{
    if (openFiles() != e_success)
    {
        cout << "ERROR : File open failed"
             << endl;

        return e_failure;
    }

    cout << "Files opened successfully"
         << endl;

    fseek(fptr_stego_image,
          54,
          SEEK_SET);

    cout << "BMP Header skipped"
         << endl;

    if (decodeMagicString()
        != e_success)
    {
        cout << "Magic String mismatch"
             << endl;

        return e_failure;
    }

    cout << "Magic String verified"
         << endl;

    if (decodeSecretFileExtnSize()
        != e_success)
    {
        return e_failure;
    }

    cout << "Extension size decoded"
         << endl;

    if (decodeSecretFileExtn()
        != e_success)
    {
        return e_failure;
    }

    cout << "Extension decoded : "
         << extn_secret_file
         << endl;

    if (decodeSecretFileSize()
        != e_success)
    {
        return e_failure;
    }

    cout << "File size decoded : "
         << size_secret_file
         << endl;

    if (decodeSecretFileData()
        != e_success)
    {
        return e_failure;
    }

    cout << "Secret data decoded"
         << endl;

    fclose(fptr_stego_image);
    fclose(fptr_output_file);

    return e_success;
}