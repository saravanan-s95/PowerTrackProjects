#include "main.h"

static unsigned int convert_size(unsigned int size)
{
    return ((size >> 24) & 0x000000FF) |
           ((size >> 8)  & 0x0000FF00) |
           ((size << 8)  & 0x00FF0000) |
           ((size << 24) & 0xFF000000);
}

static void edit_frame(MP3Info *mp3, char *frame_id, char *new_data)
{
    FILE *fp = fopen(mp3->filename, "r+");

    if(fp == NULL)
    {
        printf("ERROR : Unable to open %s\n", mp3->filename);
        return;
    }

    char id[5];
    id[4] = '\0';

    unsigned int size;

    // skip ID3 Header
    fseek(fp, ID3_HEADER_SIZE, SEEK_SET);

    while(fread(id, 1, 4, fp) == 4)
    {
        fread(&size, 4, 1, fp);

        size = convert_size(size);

        // skip frame flags
        fseek(fp, 2, SEEK_CUR);

        if(strcmp(id, frame_id) == 0)
        {
            long data_pos = ftell(fp);

            // first byte = encoding
            unsigned char encoding;
            fread(&encoding, 1, 1, fp);

            if(strlen(new_data) > (size - 1))
            {
                printf("ERROR : New data exceeds frame size\n");
                fclose(fp);
                return;
            }

            fseek(fp, data_pos + 1, SEEK_SET);

            fwrite(new_data, 1, strlen(new_data), fp);

            // fill remaining bytes with NULL 
            int remaining = (size - 1) - strlen(new_data);

            while(remaining--)
            {
                fputc('\0', fp);
            }

            printf("INFO : %s updated successfully\n", frame_id);

            fclose(fp);
            return;
        }

        // skip frame data 
        fseek(fp, size, SEEK_CUR);
    }

    printf("ERROR : Frame %s not found\n", frame_id);

    fclose(fp);
}

void edit_title(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, TITLE_FRAME, new_data);
}

void edit_artist(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, ARTIST_FRAME, new_data);
}

void edit_album(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, ALBUM_FRAME, new_data);
}

void edit_year(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, YEAR_FRAME, new_data);
}

void edit_genre(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, GENRE_FRAME, new_data);
}

void edit_comment(MP3Info *mp3, char *new_data)
{
    edit_frame(mp3, COMMENT_FRAME, new_data);
}