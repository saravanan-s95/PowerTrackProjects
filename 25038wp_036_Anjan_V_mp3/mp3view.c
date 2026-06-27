#include "main.h"


static unsigned int convert_size(unsigned int size)
{
    return ((size >> 24) & 0x000000FF) |
           ((size >> 8)  & 0x0000FF00) |
           ((size << 8)  & 0x00FF0000) |
           ((size << 24) & 0xFF000000);
}

static void read_frame(FILE *fp, const char *frame_id, char *buffer)
{
    char id[5];
    unsigned int size;

    id[4] = '\0';

    fseek(fp, ID3_HEADER_SIZE, SEEK_SET);

    while(fread(id, 1, 4, fp) == 4)
    {
        fread(&size, 4, 1, fp);
        size = convert_size(size);

        /* Skip flags */
        fseek(fp, 2, SEEK_CUR);

        if(strcmp(id, frame_id) == 0)
        {
            /* Skip encoding byte */
            fgetc(fp);

            fread(buffer, 1, size - 1, fp);
            buffer[size - 1] = '\0';
            return;
        }

        fseek(fp, size, SEEK_CUR);
    }

    strcpy(buffer, "Not Found");
}

void view_tags(MP3Info *mp3)
{
    FILE *fp = fopen(mp3->filename, "rb");

    if(fp == NULL)
    {
        printf("ERROR: Unable to open file %s\n", mp3->filename);
        return;
    }

    read_frame(fp, TITLE_FRAME, mp3->title);

    rewind(fp);
    read_frame(fp, ARTIST_FRAME, mp3->artist);

    rewind(fp);
    read_frame(fp, ALBUM_FRAME, mp3->album);

    rewind(fp);
    read_frame(fp, YEAR_FRAME, mp3->year);

    rewind(fp);
    read_frame(fp, GENRE_FRAME, mp3->genre);

    printf("\n----------------------------------------\n");
    printf("MP3 TAG INFORMATION\n");
    printf("----------------------------------------\n");
    printf("Title  : %s\n", mp3->title);
    printf("Artist : %s\n", mp3->artist);
    printf("Album  : %s\n", mp3->album);
    printf("Year   : %s\n", mp3->year);
    printf("Genre  : %s\n", mp3->genre);
    printf("----------------------------------------\n");

    fclose(fp);
}