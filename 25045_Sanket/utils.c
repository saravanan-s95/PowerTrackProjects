#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

TagData *create_tag_data(void)
{
    return (TagData *)calloc(1, sizeof(TagData));
}

static void free_string(char **ptr)
{
    if(ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void free_tag_data(TagData *data)
{
    if(!data)
        return;

    free_string(&data->title);
    free_string(&data->artist);
    free_string(&data->album);
    free_string(&data->year);
    free_string(&data->comment);
    free_string(&data->genre);
    free_string(&data->track);
    free_string(&data->album_artist);
    free_string(&data->composer);
    free_string(&data->copyright);
    free_string(&data->publisher);

    free_string(&data->album_art.mime_type);

    if(data->album_art.image_data)
    {
        free(data->album_art.image_data);
        data->album_art.image_data = NULL;
    }

    free(data);
}

char *safe_strdup(const char *src)
{
    if(!src)
        return NULL;

    size_t len = strlen(src);

    char *dst = malloc(len + 1);

    if(!dst)
        return NULL;

    memcpy(dst, src, len + 1);

    return dst;
}

uint32_t big_endian_to_host(const uint8_t *bytes)
{
    return ((uint32_t)bytes[0] << 24) |
           ((uint32_t)bytes[1] << 16) |
           ((uint32_t)bytes[2] << 8)  |
            (uint32_t)bytes[3];
}

void host_to_big_endian(uint32_t value, uint8_t *bytes)
{
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8)  & 0xFF;
    bytes[3] = value & 0xFF;
}

uint32_t synchsafe_to_int(const uint8_t *bytes)
{
    return ((uint32_t)bytes[0] << 21) |
           ((uint32_t)bytes[1] << 14) |
           ((uint32_t)bytes[2] << 7)  |
            (uint32_t)bytes[3];
}

void int_to_synchsafe(uint32_t value, uint8_t *bytes)
{
    bytes[0] = (value >> 21) & 0x7F;
    bytes[1] = (value >> 14) & 0x7F;
    bytes[2] = (value >> 7)  & 0x7F;
    bytes[3] = value & 0x7F;
}

int is_supported_version(uint8_t version)
{
    return (version == ID3_V23 || version == ID3_V24);
}

int is_valid_frame_id(const char *frame_id)
{
    if(!frame_id)
        return 0;

    for(int i = 0; i < 4; i++)
    {
        if(frame_id[i] == '\0')
            return 0;
    }

    return 1;
}