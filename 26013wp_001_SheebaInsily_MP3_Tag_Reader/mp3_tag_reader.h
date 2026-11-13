#ifndef MP3_TAG_READER_H
#define MP3_TAG_READER_H

#include <stdio.h>

int check_operation_type(char *argv[]);

void view_tag(char *filename);

void edit_tag(char *filename,
              char *title,
              char *artist,
              char *album,
              char *year,
              char *genre);

#endif
