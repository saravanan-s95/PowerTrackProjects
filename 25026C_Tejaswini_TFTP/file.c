/*
 * file.c - File I/O utilities implementation
 */
#include <stdio.h>
#include "file.h"

static FILE *fp = NULL;

int file_open_read(char *filename)
{
    fp = fopen(filename, "rb");
    if (fp == NULL) return -1;
    return 0;
}

int file_open_write(char *filename)
{
    fp = fopen(filename, "wb");
    if (fp == NULL) return -1;
    return 0;
}

int file_buffer_from_pos(char *buffer, int size)
{
    if (fp == NULL) return -1;
    int bytes_read = fread(buffer, 1, size, fp);
    return bytes_read;
}

int file_append_from_buffer(char *buffer, int size)
{
    if (fp == NULL) return -1;
    int bytes_written = fwrite(buffer, 1, size, fp);
    return bytes_written;
}

int file_close(void)
{
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}