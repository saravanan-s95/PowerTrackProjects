#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "file.h"
#include "tftp.h"

/* Open file for reading */
int file_open_read(char *filename)
{
    int fd;

    fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    return fd;
}

/* Read data from file */
int file_buffer_from_pos(int fd, char *buffer, int size)
{
    int bytes;

    bytes = read(fd, buffer, size);

    if (bytes < 0)
    {
        perror("read");
        return -1;
    }

    return bytes;
}

/* Open file for writing */
int file_open_write(char *filename)
{
    int fd;

    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    return fd;
}

/* Write buffer into file */
int file_append_from_buffer(int fd, char *buffer, int size)
{
    int bytes;

    bytes = write(fd, buffer, size);

    if (bytes < 0)
    {
        perror("write");
        return -1;
    }

    return bytes;
}

/* Close file */
int file_close(int fd)
{
    return close(fd);
}