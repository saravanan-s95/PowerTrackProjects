
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3_tag_reader.h"

int edit_tag(char *new_value, char *filename, char *frame_id)
{
    FILE *fp;
    FILE *temp_fp;
    unsigned char header[10];
    unsigned char frame_header[10];
    char current_frame[5];
    unsigned char *frame_data;
    unsigned int old_tag_size;
    unsigned int new_tag_size = 0;
    unsigned int frame_size;
    unsigned int bytes_read = 0;
    unsigned int i;
    unsigned int new_frame_size;
    unsigned int found = 0;
    unsigned char size_bytes[4];
    unsigned char buffer[4096];
    size_t count;
    long audio_position;

    if(frame_id[0] != 'T')
    {
        printf("Only text frames can be edited\n");
        return FAILURE;
    }

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

    old_tag_size = ((unsigned int)header[6] << 21) |
                   ((unsigned int)header[7] << 14) |
                   ((unsigned int)header[8] << 7) |
                   (unsigned int)header[9];

    audio_position = 10L + old_tag_size;

    temp_fp = fopen("temp.mp3", "wb");

    if(temp_fp == NULL)
    {
        printf("Unable to create temporary file\n");
        fclose(fp);
        return FAILURE;
    }

    if(fwrite(header, 1, 10, temp_fp) != 10)
    {
        printf("Unable to write ID3 header\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    while(bytes_read + 10 <= old_tag_size)
    {
        if(fread(frame_header, 1, 10, fp) != 10)
        {
            printf("Unable to read frame header\n");
            fclose(fp);
            fclose(temp_fp);
            remove("temp.mp3");
            return FAILURE;
        }

        if(frame_header[0] == 0)
        {
            break;
        }

        for(i = 0; i < 4; i++)
        {
            current_frame[i] = frame_header[i];
        }

        current_frame[4] = '\0';

        frame_size = ((unsigned int)frame_header[4] << 24) |
                     ((unsigned int)frame_header[5] << 16) |
                     ((unsigned int)frame_header[6] << 8) |
                     (unsigned int)frame_header[7];

        if(frame_size > old_tag_size - bytes_read - 10)
        {
            printf("Invalid frame size\n");
            fclose(fp);
            fclose(temp_fp);
            remove("temp.mp3");
            return FAILURE;
        }

        frame_data = malloc(frame_size + 1);

        if(frame_data == NULL)
        {
            printf("Memory allocation failed\n");
            fclose(fp);
            fclose(temp_fp);
            remove("temp.mp3");
            return FAILURE;
        }

        if(fread(frame_data, 1, frame_size, fp) != frame_size)
        {
            printf("Unable to read frame data\n");
            free(frame_data);
            fclose(fp);
            fclose(temp_fp);
            remove("temp.mp3");
            return FAILURE;
        }

        if(strcmp(current_frame, frame_id) == 0)
        {
            if(frame_size < 1)
            {
                printf("Invalid text frame\n");
                free(frame_data);
                fclose(fp);
                fclose(temp_fp);
                remove("temp.mp3");
                return FAILURE;
            }

            new_frame_size = (unsigned int)strlen(new_value) + 1;

            size_bytes[0] = (new_frame_size >> 24) & 0xFF;
            size_bytes[1] = (new_frame_size >> 16) & 0xFF;
            size_bytes[2] = (new_frame_size >> 8) & 0xFF;
            size_bytes[3] = new_frame_size & 0xFF;

            if(fwrite(frame_header, 1, 4, temp_fp) != 4 ||
               fwrite(size_bytes, 1, 4, temp_fp) != 4 ||
               fwrite(frame_header + 8, 1, 2, temp_fp) != 2 ||
               fwrite(frame_data, 1, 1, temp_fp) != 1 ||
               fwrite(new_value, 1, new_frame_size - 1, temp_fp)
                   != new_frame_size - 1)
            {
                printf("Unable to write updated frame\n");
                free(frame_data);
                fclose(fp);
                fclose(temp_fp);
                remove("temp.mp3");
                return FAILURE;
            }

            new_tag_size += 10 + new_frame_size;
            found = 1;
        }
        else
        {
            if(fwrite(frame_header, 1, 10, temp_fp) != 10 ||
               fwrite(frame_data, 1, frame_size, temp_fp) != frame_size)
            {
                printf("Unable to copy frame\n");
                free(frame_data);
                fclose(fp);
                fclose(temp_fp);
                remove("temp.mp3");
                return FAILURE;
            }

            new_tag_size += 10 + frame_size;
        }

        free(frame_data);

        bytes_read += 10 + frame_size;
    }

    if(!found)
    {
        printf("Requested frame not found\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    if(new_tag_size > 0x0FFFFFFF)
    {
        printf("Updated tag is too large\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    header[6] = (new_tag_size >> 21) & 0x7F;
    header[7] = (new_tag_size >> 14) & 0x7F;
    header[8] = (new_tag_size >> 7) & 0x7F;
    header[9] = new_tag_size & 0x7F;

    if(fseek(temp_fp, 0, SEEK_SET) != 0 ||
       fwrite(header, 1, 10, temp_fp) != 10)
    {
        printf("Unable to update tag size\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    if(fseek(fp, audio_position, SEEK_SET) != 0 ||
       fseek(temp_fp, 10L + new_tag_size, SEEK_SET) != 0)
    {
        printf("Unable to locate audio data\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    while((count = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        if(fwrite(buffer, 1, count, temp_fp) != count)
        {
            printf("Unable to copy audio data\n");
            fclose(fp);
            fclose(temp_fp);
            remove("temp.mp3");
            return FAILURE;
        }
    }

    if(ferror(fp))
    {
        printf("Unable to read audio data\n");
        fclose(fp);
        fclose(temp_fp);
        remove("temp.mp3");
        return FAILURE;
    }

    if(fclose(fp) != 0)
    {
        fclose(temp_fp);
        remove("temp.mp3");
        printf("Unable to close original file\n");
        return FAILURE;
    }

    if(fclose(temp_fp) != 0)
    {
        remove("temp.mp3");
        printf("Unable to close temporary file\n");
        return FAILURE;
    }

    if(rename("temp.mp3", filename) != 0)
    {
        printf("Unable to replace original file\n");
        remove("temp.mp3");
        return FAILURE;
    }

    printf("Tag updated successfully\n");

    return SUCCESS;
}
