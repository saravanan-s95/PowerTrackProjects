#ifndef MP3_HEADER_H
#define MP3_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>


#define VIEW 	1
#define EDIT 	2

#define SUCCESS  0
#define FAILURE -1

#define HEADER_SIZE 10
#define ID_SIZE 4
#define FRAME_SIZE 4
#define FLAG_OFFSET 2

typedef struct MP3
{
    char *mp3_filename;
    /* For edit functions */
    char *new_value;
    char *tag_id;
}MP3_t;

int validate_cla(int argc, char *argv[], MP3_t *mp3);
void view(MP3_t *mp3);
void edit(MP3_t *mp3);
void print_tag_data(FILE *);
char* tag_to_name(const char *);

void print_help(void);
int flag_validate(const char *);
char *tag_id_to_label(char *);

#endif
