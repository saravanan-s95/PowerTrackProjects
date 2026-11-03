#ifndef MP3_TAG_READER_H
#define MP3_TAG_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1

typedef struct
{
    char title[100];
    char artist[100];
    char album[100];
    char year[10];
    char genre[50];
    char track[10];
    char comment[200];
} MP3Tag;

int view_tags(char *filename);
int edit_tag(char *new_value, char *filename, char *frame_id);

#endif