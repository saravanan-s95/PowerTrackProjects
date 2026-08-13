#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum
{
	VIEW,
	EDIT,
	HELP,
	UNSUPPORTED
}Operation_type;

typedef enum
{
	e_success,
	e_failure
}Status;
typedef struct
{
    FILE *fileptr;

    char *filename;

    char frame_id[5];

    unsigned int frame_size;

    char tag_data[200];

    char edit_frame[5];

    char *new_data;

    unsigned char frame_flags[2];

} Mp3Info;
Operation_type check_opern_type(char *argv[]);
void print_help();
Status view_details(Mp3Info *mp3info);
Status edit_detail(Mp3Info *mp3info);
Status check_head (Mp3Info *mp3info);
Status check_version(Mp3Info *mp3info);
Status read_frame(Mp3Info *mp3info);
void print_view_header(void);
void display_tag(Mp3Info *mp3info);
void print_view_footer(void);
Status check_extension(Mp3Info *mp3info);
Status open_files(Mp3Info *mp3info);
#endif
