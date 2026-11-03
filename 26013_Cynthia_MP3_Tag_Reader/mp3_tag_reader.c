
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3_tag_reader.h"
#include "display.h"

void copy_tag_value(char *destination, char *source, int size)
{
    int i = 0;

    while(source[i] != '\0' && i < size - 1)
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';
}

int view_tags(char *filename)
{
    FILE *fp;
    unsigned char header[10];
    unsigned char frame_header[10];
    unsigned int tag_size;
    unsigned int frame_size;
    unsigned int bytes_read = 0;
    char frame_id[5];
    char *frame_data;
    MP3Tag tag = {0};
    unsigned int i;
    unsigned int j;

    fp = fopen(filename, "rb");

    if(fp == NULL)
    {
        printf("Unable to open MP3 file\n");
        return FAILURE;
    }

    if(fread(header, 1, 10, fp) != 10)
    {
        printf("Unable to read ID3 header\n");
        fclose(fp);
        return FAILURE;
    }

    if(header[0] != 'I' || header[1] != 'D' || header[2] != '3')
    {
        printf("ID3 tag not found\n");
        fclose(fp);
        return FAILURE;
    }

    if(header[3] != 3 || header[4] != 0)
    {
        printf("Only ID3v2.3.0 is supported\n");
        fclose(fp);
        return FAILURE;
    }

    tag_size = ((unsigned int)(header[6] & 0x7F) << 21) |
               ((unsigned int)(header[7] & 0x7F) << 14) |
               ((unsigned int)(header[8] & 0x7F) << 7) |
               (unsigned int)(header[9] & 0x7F);

    while(bytes_read + 10 <= tag_size)
    {
        if(fread(frame_header, 1, 10, fp) != 10)
        {
            break;
        }

        if(frame_header[0] == 0)
        {
            break;
        }

        for(i = 0; i < 4; i++)
        {
            frame_id[i] = frame_header[i];
        }

        frame_id[4] = '\0';

        frame_size = ((unsigned int)frame_header[4] << 24) |
                     ((unsigned int)frame_header[5] << 16) |
                     ((unsigned int)frame_header[6] << 8) |
                     (unsigned int)frame_header[7];

        if(frame_size == 0 || frame_size > tag_size - bytes_read - 10)
        {
            break;
        }

        frame_data = malloc(frame_size + 1);

        if(frame_data == NULL)
        {
            printf("Memory allocation failed\n");
            fclose(fp);
            return FAILURE;
        }

        if(fread(frame_data, 1, frame_size, fp) != frame_size)
        {
            free(frame_data);
            fclose(fp);
            printf("Unable to read frame data\n");
            return FAILURE;
        }

        frame_data[frame_size] = '\0';

        if(frame_size > 1 && frame_data[0] == 0)
        {
            if(strcmp(frame_id, "TIT2") == 0)
            {
                copy_tag_value(tag.title, frame_data + 1,
                               sizeof(tag.title));
            }
            else if(strcmp(frame_id, "TPE1") == 0)
            {
                copy_tag_value(tag.artist, frame_data + 1,
                               sizeof(tag.artist));
            }
            else if(strcmp(frame_id, "TALB") == 0)
            {
                copy_tag_value(tag.album, frame_data + 1,
                               sizeof(tag.album));
            }
            else if(strcmp(frame_id, "TYER") == 0)
            {
                copy_tag_value(tag.year, frame_data + 1,
                               sizeof(tag.year));
            }
            else if(strcmp(frame_id, "TCON") == 0)
            {
                copy_tag_value(tag.genre, frame_data + 1,
                               sizeof(tag.genre));
            }
            else if(strcmp(frame_id, "TRCK") == 0)
            {
                copy_tag_value(tag.track, frame_data + 1,
                               sizeof(tag.track));
            }
        }

        if(strcmp(frame_id, "COMM") == 0 &&
           frame_size > 4 && frame_data[0] == 0)
        {
            j = 4;

            while(j < frame_size && frame_data[j] != '\0')
            {
                j++;
            }

            if(j < frame_size && j + 1 < frame_size)
            {
                copy_tag_value(tag.comment, frame_data + j + 1,
                               sizeof(tag.comment));
            }
        }

        free(frame_data);

        bytes_read += 10 + frame_size;
    }

    display_tags(&tag);

    fclose(fp);

    return SUCCESS;
}
