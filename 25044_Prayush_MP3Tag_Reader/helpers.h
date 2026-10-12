#ifndef HELPER_H
#define HELPER_H

#include "types.h"
#include <stdio.h>

typedef struct MP3_info {
    char *file_name;
    char tag_to_edit;
    char *content;
    char *temp_file_name;
} MP3_info;

#define MP3_MAGIC "ID3"

Status readIntFromBuffer(unsigned int *a, unsigned char *buffer);
Status readSynchSafeIntFromBuffer(unsigned int *a, unsigned char *buffer);
Status writeIntToBuffer(const unsigned int *a, unsigned char *buffer);
Status writeSynchSafeIntToBuffer(const unsigned int *a, unsigned char *buffer);

Status readInt(int *value);
#endif // !HELPER_H
