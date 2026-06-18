#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>

typedef enum
{
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_INVALID_ID3,
    ERR_UNSUPPORTED_VERSION,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_FRAME,
    ERR_TAG_NOT_FOUND
} ErrorCode;

/* Error reporting */
void display_error(const char *message);
void display_system_error(const char *message);

/* File validation */
int file_exists(const char *filename);
int check_id3_tag_presence(const char *filename);

/* Generic validations */
int validate_mp3_file(const char *filename);

#endif