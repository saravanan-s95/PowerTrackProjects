#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writer.h"
#include "reader.h"
#include "utils.h"
#include "error_handling.h"

#define PADDING_SIZE 1024
#define COPY_BUFFER_SIZE 8192

static int safe_copy(FILE *src, FILE *dst)
{
    unsigned char buffer[COPY_BUFFER_SIZE];

    size_t bytes_read;

    while ((bytes_read =
            fread(buffer,
                  1,
                  sizeof(buffer),
                  src)) > 0)
    {
        if (fwrite(buffer,
                   1,
                   bytes_read,
                   dst) != bytes_read)
        {
            return -1;
        }
    }

    return ferror(src) ? -1 : 0;
}

static uint32_t calculate_text_frame_size(
                    const char *text)
{
    if (!text)
        return 0;

    return 10 + 1 + strlen(text);
}

static uint32_t calculate_apic_size(
                    const TagData *data)
{
    if (!data->album_art.image_data)
        return 0;

    const char *mime =
        data->album_art.mime_type ?
        data->album_art.mime_type :
        "image/jpeg";

    return 10 +
           1 +
           strlen(mime) + 1 +
           1 +
           1 +
           data->album_art.image_size;
}

static uint32_t calculate_total_tag_size(
                    const TagData *data)
{
    uint32_t size = 0;

#define ADD_FRAME(field) \
    if(field) \
        size += calculate_text_frame_size(field)

    ADD_FRAME(data->title);
    ADD_FRAME(data->artist);
    ADD_FRAME(data->album);
    ADD_FRAME(data->year);
    ADD_FRAME(data->comment);
    ADD_FRAME(data->genre);
    ADD_FRAME(data->track);
    ADD_FRAME(data->album_artist);
    ADD_FRAME(data->composer);
    ADD_FRAME(data->copyright);
    ADD_FRAME(data->publisher);

#undef ADD_FRAME

    size += calculate_apic_size(data);
    size += PADDING_SIZE;

    return size;
}

static int write_text_frame(
            FILE *fp,
            const char *frame_id,
            const char *value)
{
    if (!value)
        return 0;

    uint8_t size_bytes[4];

    uint32_t frame_size =
        1 + strlen(value);

    fwrite(frame_id, 1, 4, fp);

    int_to_synchsafe(
        frame_size,
        size_bytes);

    fwrite(size_bytes, 1, 4, fp);

    uint8_t flags[2] = {0};

    fwrite(flags, 1, 2, fp);

    uint8_t encoding = 0x00;

    fwrite(&encoding, 1, 1, fp);

    fwrite(value,
           1,
           strlen(value),
           fp);

    return 0;
}

static int write_apic_frame(
                FILE *fp,
                const TagData *data)
{
    if (!data->album_art.image_data)
        return 0;

    const char *mime =
        data->album_art.mime_type ?
        data->album_art.mime_type :
        "image/jpeg";

    uint32_t frame_size =
        1 +
        strlen(mime) + 1 +
        1 +
        1 +
        data->album_art.image_size;

    uint8_t size_bytes[4];

    fwrite("APIC", 1, 4, fp);

    int_to_synchsafe(
        frame_size,
        size_bytes);

    fwrite(size_bytes, 1, 4, fp);

    uint8_t flags[2] = {0};

    fwrite(flags, 1, 2, fp);

    uint8_t encoding = 0;

    fwrite(&encoding, 1, 1, fp);

    fwrite(mime,
           1,
           strlen(mime) + 1,
           fp);

    uint8_t pic_type = 0x03;

    fwrite(&pic_type, 1, 1, fp);

    uint8_t desc = 0;

    fwrite(&desc, 1, 1, fp);

    fwrite(data->album_art.image_data,
           1,
           data->album_art.image_size,
           fp);

    return 0;
}

int write_id3_tags(
        const char *filename,
        const TagData *data)
{
    FILE *src;
    FILE *tmp;

    char temp_file[512];

    snprintf(temp_file,
             sizeof(temp_file),
             "%s.tmp",
             filename);

    src = fopen(filename, "rb");

    if (!src)
    {
        display_system_error(
            "Unable to open source file");
        return 1;
    }

    tmp = fopen(temp_file, "wb");

    if (!tmp)
    {
        fclose(src);

        display_system_error(
            "Unable to create temp file");
        return 1;
    }

    uint32_t tag_size =
        calculate_total_tag_size(data);

    fwrite("ID3", 1, 3, tmp);

    uint8_t version[2] =
    {
        0x04,
        0x00
    };

    fwrite(version, 1, 2, tmp);

    uint8_t flags = 0;

    fwrite(&flags, 1, 1, tmp);

    uint8_t size_bytes[4];

    int_to_synchsafe(
        tag_size,
        size_bytes);

    fwrite(size_bytes, 1, 4, tmp);

    write_text_frame(tmp,
                     "TIT2",
                     data->title);

    write_text_frame(tmp,
                     "TPE1",
                     data->artist);

    write_text_frame(tmp,
                     "TALB",
                     data->album);

    write_text_frame(tmp,
                     "TYER",
                     data->year);

    write_text_frame(tmp,
                     "COMM",
                     data->comment);

    write_text_frame(tmp,
                     "TCON",
                     data->genre);

    write_text_frame(tmp,
                     "TRCK",
                     data->track);

    write_text_frame(tmp,
                     "TPE2",
                     data->album_artist);

    write_text_frame(tmp,
                     "TCOM",
                     data->composer);

    write_text_frame(tmp,
                     "TCOP",
                     data->copyright);

    write_text_frame(tmp,
                     "TPUB",
                     data->publisher);

    write_apic_frame(tmp, data);

    unsigned char zero = 0;

    for (int i = 0;
         i < PADDING_SIZE;
         i++)
    {
        fwrite(&zero, 1, 1, tmp);
    }

    unsigned char header[10];

    if (fread(header, 1, 10, src) == 10)
    {
        uint32_t old_tag_size =
            synchsafe_to_int(
                &header[6]);

        fseek(src,
              old_tag_size + 10,
              SEEK_SET);
    }

    if (safe_copy(src, tmp) != 0)
    {
        fclose(src);
        fclose(tmp);

        remove(temp_file);

        return 1;
    }

    fclose(src);
    fclose(tmp);

    if (remove(filename) != 0)
    {
        remove(temp_file);
        return 1;
    }

    if (rename(temp_file,
               filename) != 0)
    {
        remove(temp_file);
        return 1;
    }

    return 0;
}

static char **find_field(
            TagData *data,
            const char *tag)
{
    if (!strcmp(tag,"-t"))
        return &data->title;

    if (!strcmp(tag,"-a"))
        return &data->artist;

    if (!strcmp(tag,"-A"))
        return &data->album;

    if (!strcmp(tag,"-y"))
        return &data->year;

    if (!strcmp(tag,"-c"))
        return &data->comment;

    if (!strcmp(tag,"-g"))
        return &data->genre;

    if (!strcmp(tag,"-T"))
        return &data->track;

    if (!strcmp(tag,"-b"))
        return &data->album_artist;

    if (!strcmp(tag,"-m"))
        return &data->composer;

    if (!strcmp(tag,"-o"))
        return &data->copyright;

    if (!strcmp(tag,"-p"))
        return &data->publisher;

    return NULL;
}

int edit_tag(
        const char *filename,
        const char *tag,
        const char *value)
{
    TagData *data =
        read_id3_tags(filename);

    if (!data)
        return 1;

    char **field =
        find_field(data, tag);

    if (!field)
    {
        free_tag_data(data);
        return 1;
    }

    free(*field);

    *field = safe_strdup(value);

    int status =
        write_id3_tags(filename,
                       data);

    free_tag_data(data);

    return status;
}

int delete_tag(
        const char *filename,
        const char *tag)
{
    TagData *data =
        read_id3_tags(filename);

    if (!data)
        return 1;

    char **field =
        find_field(data, tag);

    if (!field)
    {
        free_tag_data(data);
        return 1;
    }

    free(*field);
    *field = NULL;

    int status =
        write_id3_tags(filename,
                       data);

    free_tag_data(data);

    return status;
}

int delete_all_tags(
        const char *filename)
{
    FILE *src =
        fopen(filename, "rb");

    if (!src)
        return 1;

    FILE *tmp =
        fopen("strip.tmp", "wb");

    if (!tmp)
    {
        fclose(src);
        return 1;
    }

    unsigned char header[10];

    if (fread(header,1,10,src) != 10)
    {
        fclose(src);
        fclose(tmp);
        return 1;
    }

    uint32_t tag_size =
        synchsafe_to_int(
            &header[6]);

    fseek(src,
          tag_size + 10,
          SEEK_SET);

    safe_copy(src,tmp);

    fclose(src);
    fclose(tmp);

    remove(filename);
    rename("strip.tmp",
           filename);

    return 0;
}

int extract_album_art(
        const char *mp3_filename,
        const char *output_image_name)
{
    TagData *data =
        read_id3_tags(mp3_filename);

    if (!data)
        return 1;

    if (!data->album_art.image_data)
    {
        free_tag_data(data);
        return 1;
    }

    FILE *fp =
        fopen(output_image_name,
              "wb");

    if (!fp)
    {
        free_tag_data(data);
        return 1;
    }

    fwrite(
        data->album_art.image_data,
        1,
        data->album_art.image_size,
        fp);

    fclose(fp);

    free_tag_data(data);

    return 0;
}