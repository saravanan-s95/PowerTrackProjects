#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

#define ID3_V23 3
#define ID3_V24 4

#define ID3_HEADER_SIZE 10
#define ID3_FRAME_HEADER_SIZE 10

typedef struct
{
    char file_identifier[3];
    uint16_t version;
    uint8_t flags;
    uint32_t size;
} ID3Header;

typedef struct
{
    char frame_id[5];
    uint32_t size;
    uint16_t flags;
} FrameHeader;

typedef struct
{
    char *mime_type;
    uint32_t image_size;
    unsigned char *image_data;
} AlbumArt;

typedef struct
{
    char *title;
    char *artist;
    char *album;
    char *year;
    char *comment;
    char *genre;
    char *track;
    char *album_artist;
    char *composer;
    char *copyright;
    char *publisher;

    uint16_t tag_version;
    uint32_t padding_size;

    AlbumArt album_art;
} TagData;

/* Memory Management */
TagData *create_tag_data(void);
void free_tag_data(TagData *data);

/* String Helpers */
char *safe_strdup(const char *src);

/* Conversion Helpers */
uint32_t big_endian_to_host(const uint8_t *bytes);
void host_to_big_endian(uint32_t value, uint8_t *bytes);

uint32_t synchsafe_to_int(const uint8_t *bytes);
void int_to_synchsafe(uint32_t value, uint8_t *bytes);

/* Validation */
int is_supported_version(uint8_t version);
int is_valid_frame_id(const char *frame_id);

#endif