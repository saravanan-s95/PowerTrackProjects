#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "utils.h"
#include "error_handling.h"

#define MAX_FRAME_SIZE (16 * 1024 * 1024)

static int read_exact(FILE *fp, void *buffer, size_t size)
{
    return fread(buffer, 1, size, fp) == size;
}

int validate_frame_header(const unsigned char *frame_header)
{
    if(frame_header == NULL)
        return 0;

    if(frame_header[0] == 0x00)
        return 0;

    return 1;
}

void utf16_print(const unsigned char *buffer, uint32_t size)
{
    if(buffer == NULL || size < 3)
        return;

    uint32_t i;

    for(i = 3; i < size; i += 2)
    {
        if(buffer[i] == '\0')
            break;

        putchar(buffer[i]);
    }

    putchar('\n');
}

char *decode_text_frame(const unsigned char *buffer, uint32_t size)
{
    if(buffer == NULL || size == 0)
        return NULL;

    uint8_t encoding = buffer[0];

    char *output = calloc(size + 1, 1);

    if(output == NULL)
        return NULL;

    if(encoding == 0x00 || encoding == 0x03)
    {
        memcpy(output, buffer + 1, size - 1);
        return output;
    }

    if(encoding == 0x01 || encoding == 0x02)
    {
        uint32_t j = 0;

        for(uint32_t i = 3; i < size; i += 2)
        {
            if(buffer[i] == 0)
                break;

            output[j++] = buffer[i];
        }

        output[j] = '\0';
        return output;
    }

    free(output);
    return NULL;
}

static void assign_text_frame(
        TagData *data,
        const char *frame_id,
        char *value)
{
    if(strcmp(frame_id, "TIT2") == 0)
        data->title = value;

    else if(strcmp(frame_id, "TPE1") == 0)
        data->artist = value;

    else if(strcmp(frame_id, "TALB") == 0)
        data->album = value;

    else if(strcmp(frame_id, "TYER") == 0 ||
            strcmp(frame_id, "TDRC") == 0)
        data->year = value;

    else if(strcmp(frame_id, "COMM") == 0)
        data->comment = value;

    else if(strcmp(frame_id, "TCON") == 0)
        data->genre = value;

    else if(strcmp(frame_id, "TRCK") == 0)
        data->track = value;

    else if(strcmp(frame_id, "TPE2") == 0)
        data->album_artist = value;

    else if(strcmp(frame_id, "TCOM") == 0)
        data->composer = value;

    else if(strcmp(frame_id, "TCOP") == 0)
        data->copyright = value;

    else if(strcmp(frame_id, "TPUB") == 0)
        data->publisher = value;

    else
        free(value);
}

TagData *read_id3_tags(const char *filename)
{
    FILE *fp;
    unsigned char header[10];

    fp = fopen(filename, "rb");

    if(fp == NULL)
    {
        display_system_error("Unable to open MP3 file");
        return NULL;
    }

    if(!read_exact(fp, header, sizeof(header)))
    {
        fclose(fp);
        display_error("Unable to read ID3 header");
        return NULL;
    }

    if(memcmp(header, "ID3", 3) != 0)
    {
        fclose(fp);
        display_error("ID3 tag not found");
        return NULL;
    }

    TagData *data = create_tag_data();

    if(data == NULL)
    {
        fclose(fp);
        display_error("Memory allocation failed");
        return NULL;
    }

    data->tag_version = header[3];

    uint32_t tag_size = synchsafe_to_int(&header[6]);
    uint32_t bytes_read = 0;

    while(bytes_read < tag_size)
    {
        unsigned char frame_header[10];

        if(!read_exact(fp, frame_header, 10))
            break;

        bytes_read += 10;

        if(frame_header[0] == 0)
        {
            data->padding_size =
                tag_size - bytes_read + 10;
            break;
        }

        char frame_id[5];
        memcpy(frame_id, frame_header, 4);
        frame_id[4] = '\0';

        uint32_t frame_size;

        if(data->tag_version == ID3_V23)
            frame_size =
                big_endian_to_host(&frame_header[4]);
        else
            frame_size =
                synchsafe_to_int(&frame_header[4]);

        if(frame_size == 0 ||
           frame_size > MAX_FRAME_SIZE)
        {
            display_error("Invalid frame size");
            break;
        }

        unsigned char *frame_data =
            malloc(frame_size);

        if(frame_data == NULL)
        {
            display_error("Memory allocation failed");
            break;
        }

        if(!read_exact(fp, frame_data, frame_size))
        {
            free(frame_data);
            break;
        }

        bytes_read += frame_size;

        if(frame_id[0] == 'T' ||
           strcmp(frame_id, "COMM") == 0)
        {
            char *decoded =
                decode_text_frame(
                    frame_data,
                    frame_size);

            if(decoded)
                assign_text_frame(
                    data,
                    frame_id,
                    decoded);
        }
        else if(strcmp(frame_id, "APIC") == 0)
        {
            data->album_art.image_size =
                frame_size;

            data->album_art.image_data =
                malloc(frame_size);

            if(data->album_art.image_data)
            {
                memcpy(
                    data->album_art.image_data,
                    frame_data,
                    frame_size);
            }

            uint32_t pos = 1;

            while(pos < frame_size &&
                  frame_data[pos] != '\0')
            {
                pos++;
            }

            if(pos < frame_size)
            {
                data->album_art.mime_type =
                    safe_strdup(
                        (char *)&frame_data[1]);
            }
        }

        free(frame_data);
    }

    fclose(fp);
    return data;
}

void display_metadata(const TagData *data)
{
    if(data == NULL)
        return;

    printf("\n");
    printf("====================================\n");
    printf("       MP3 TAG INFORMATION\n");
    printf("====================================\n");

#define PRINT_FIELD(name,value) \
    if(value) \
        printf("%-15s : %s\n",name,value);

    PRINT_FIELD("Title",data->title);
    PRINT_FIELD("Artist",data->artist);
    PRINT_FIELD("Album",data->album);
    PRINT_FIELD("Year",data->year);
    PRINT_FIELD("Comment",data->comment);
    PRINT_FIELD("Genre",data->genre);
    PRINT_FIELD("Track",data->track);
    PRINT_FIELD("Album Artist",data->album_artist);
    PRINT_FIELD("Composer",data->composer);
    PRINT_FIELD("Copyright",data->copyright);
    PRINT_FIELD("Publisher",data->publisher);

#undef PRINT_FIELD

    if(data->album_art.image_data)
    {
        printf("Album Art      : Present\n");

        if(data->album_art.mime_type)
            printf("Image Type     : %s\n",
                   data->album_art.mime_type);

        printf("Image Size     : %u bytes\n",
               data->album_art.image_size);
    }
    else
    {
        printf("Album Art      : Not Present\n");
    }

    printf("====================================\n");
}

void view_tags(const char *filename)
{
    TagData *data = read_id3_tags(filename);

    if(data == NULL)
        return;

    display_metadata(data);

    free_tag_data(data);
}