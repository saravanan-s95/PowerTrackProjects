#ifndef MAIN_H
#define MAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0
#define ID3_HEADER_SIZE 10

#define TITLE_FRAME  "TIT2"
#define ARTIST_FRAME "TPE1"
#define ALBUM_FRAME  "TALB"
#define YEAR_FRAME   "TYER"
#define GENRE_FRAME  "TCON"
#define COMMENT_FRAME "COMM"


typedef struct MP3
{
    char *filename;
    FILE *fptr;

    char title[100];      // TIT2
    char artist[100];     // TPE1
    char album[100];      // TALB
    char year[10];        // TYER
    char genre[50];       // TCON
} MP3Info;


/* View */
void view_tags(MP3Info *mp3);

/* EDIT functions*/
void edit_title(MP3Info *mp3, char *new_data);
void edit_artist(MP3Info *mp3, char *new_data);
void edit_album(MP3Info *mp3, char *new_data);
void edit_year(MP3Info *mp3, char *new_data);
void edit_genre(MP3Info *mp3, char *new_data);
void edit_comment(MP3Info *mp3, char *new_data);

/* validate and error */
void mp3_error();
int validate_mp3(MP3Info *mp3);

#endif