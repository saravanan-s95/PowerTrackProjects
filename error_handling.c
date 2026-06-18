#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "error_handling.h"

void display_error(const char *message)
{
    fprintf(stderr, "[ERROR] %s\n", message);
}

void display_system_error(const char *message)
{
    fprintf(stderr,
            "[ERROR] %s : %s\n",
            message,
            strerror(errno));
}

int file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if(fp == NULL)
        return 0;

    fclose(fp);
    return 1;
}

int check_id3_tag_presence(const char *filename)
{
    FILE *fp;
    unsigned char header[10];

    fp = fopen(filename, "rb");

    if(fp == NULL)
    {
        display_system_error("Unable to open file");
        return 0;
    }

    if(fread(header, 1, sizeof(header), fp) != sizeof(header))
    {
        fclose(fp);
        display_error("File too small");
        return 0;
    }

    fclose(fp);

    if(header[0] != 'I' ||
       header[1] != 'D' ||
       header[2] != '3')
    {
        display_error("ID3 signature not found");
        return 0;
    }

    if(header[3] != 3 &&
       header[3] != 4)
    {
        display_error("Unsupported ID3 version");
        return 0;
    }

    return 1;
}

int validate_mp3_file(const char *filename)
{
    if(filename == NULL)
    {
        display_error("Filename is NULL");
        return 0;
    }

    if(!file_exists(filename))
    {
        display_error("File does not exist");
        return 0;
    }

    return 1;
}