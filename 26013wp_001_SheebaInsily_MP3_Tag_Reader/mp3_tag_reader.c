#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mp3_tag_reader.h"

/* ---------------------------------------------------------
   Operation type
   --------------------------------------------------------- */
int check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
        return 1;

    if (strcmp(argv[1], "-e") == 0)
        return 2;

    if (strcmp(argv[1], "-h") == 0)
        return 3;

    return 0;
}

/* ---------------------------------------------------------
   ID3v2.3 tag size: sync-safe integer
   --------------------------------------------------------- */
static unsigned int get_tag_size(const unsigned char b[4])
{
    return ((unsigned int)(b[0] & 0x7F) << 21) |
           ((unsigned int)(b[1] & 0x7F) << 14) |
           ((unsigned int)(b[2] & 0x7F) << 7) |
           (unsigned int)(b[3] & 0x7F);
}

/* ID3v2.3 frame size: normal big-endian integer */
static unsigned int get_frame_size(const unsigned char b[4])
{
    return ((unsigned int)b[0] << 24) |
           ((unsigned int)b[1] << 16) |
           ((unsigned int)b[2] << 8) |
           (unsigned int)b[3];
}

/* ---------------------------------------------------------
   Print a text frame.
   Supports encoding 0 (ISO-8859-1) and the common
   UTF-16LE form used by the supplied MP3.
   --------------------------------------------------------- */
static void print_text_value(const unsigned char *data, unsigned int size)
{
    unsigned int i;
    unsigned char encoding;

    if (size == 0)
        return;

    encoding = data[0];

    if (encoding == 0 || encoding == 3)
    {
        for (i = 1; i < size; i++)
        {
            if (data[i] == 0)
                break;
            putchar(data[i]);
        }
    }
    else if (encoding == 1 || encoding == 2)
    {
        unsigned int start = 1;

        if (encoding == 1 && size >= 3 &&
            ((data[1] == 0xFF && data[2] == 0xFE) ||
             (data[1] == 0xFE && data[2] == 0xFF)))
        {
            start = 3;
        }

        /*
         * For the MP3 used in this project, text is ASCII
         * represented as UTF-16LE: character byte, 00.
         */
        if (encoding == 1 &&
            start + 1 < size &&
            data[start + 1] == 0)
        {
            for (i = start; i + 1 < size; i += 2)
            {
                if (data[i] == 0 && data[i + 1] == 0)
                    break;

                putchar(data[i]);
            }
        }
        else
        {
            /* UTF-16BE / fallback */
            for (i = start; i + 1 < size; i += 2)
            {
                if (data[i] == 0 && data[i + 1] == 0)
                    break;

                if (data[i] != 0)
                    putchar(data[i]);
                else
                    putchar(data[i + 1]);
            }
        }
    }
}

/* ---------------------------------------------------------
   Print a normal text frame
   --------------------------------------------------------- */
static void print_text_frame(FILE *fp,
                             const char *frame_id,
                             unsigned int frame_size)
{
    unsigned char *data;

    if (frame_size == 0)
        return;

    data = (unsigned char *)malloc(frame_size);
    if (data == NULL)
    {
        printf("ERROR: Memory allocation failed\n");
        fseek(fp, frame_size, SEEK_CUR);
        return;
    }

    if (fread(data, 1, frame_size, fp) != frame_size)
    {
        free(data);
        return;
    }

    printf("Frame ID: %s\n", frame_id);
    printf("Frame Size: %u bytes\n", frame_size);

    if (strcmp(frame_id, "TALB") == 0)
        printf("Album: ");
    else if (strcmp(frame_id, "TPE1") == 0)
        printf("Artist: ");
    else if (strcmp(frame_id, "TIT2") == 0)
        printf("Title: ");
    else if (strcmp(frame_id, "TYER") == 0)
        printf("Year: ");
    else if (strcmp(frame_id, "TCON") == 0)
        printf("Genre: ");
    else if (strcmp(frame_id, "TRCK") == 0)
        printf("Track: ");
    else if (strcmp(frame_id, "TCOP") == 0)
        printf("Copyright: ");
    else if (strcmp(frame_id, "TPOS") == 0)
        printf("Position: ");
    else
        printf("Value: ");

    print_text_value(data, frame_size);
    printf("\n\n");

    free(data);
}

/* ---------------------------------------------------------
   Print COMM
   --------------------------------------------------------- */
static void print_comment_frame(FILE *fp, unsigned int frame_size)
{
    unsigned char *data;
    unsigned int i;
    unsigned int start;

    if (frame_size == 0)
        return;

    data = (unsigned char *)malloc(frame_size);
    if (data == NULL)
    {
        fseek(fp, frame_size, SEEK_CUR);
        return;
    }

    if (fread(data, 1, frame_size, fp) != frame_size)
    {
        free(data);
        return;
    }

    printf("Frame ID: COMM\n");
    printf("Frame Size: %u bytes\n", frame_size);

    if (frame_size >= 4)
    {
        printf("Language: %c%c%c\n",
               data[1], data[2], data[3]);

        start = 4;

        /* Skip short description */
        if (data[0] == 0 || data[0] == 3)
        {
            while (start < frame_size && data[start] != 0)
                start++;

            if (start < frame_size)
                start++;

            printf("Comment: ");

            for (i = start; i < frame_size; i++)
            {
                if (data[i] == 0)
                    break;
                putchar(data[i]);
            }
        }
        else
        {
            while (start + 1 < frame_size)
            {
                if (data[start] == 0 && data[start + 1] == 0)
                {
                    start += 2;
                    break;
                }
                start += 2;
            }

            printf("Comment: ");

            for (i = start; i + 1 < frame_size; i += 2)
            {
                if (data[i] == 0 && data[i + 1] == 0)
                    break;

                putchar(data[i] != 0 ? data[i] : data[i + 1]);
            }
        }

        printf("\n\n");
    }

    free(data);
}

/* ---------------------------------------------------------
   Print WXXX
   --------------------------------------------------------- */
static void print_wxxx_frame(FILE *fp, unsigned int frame_size)
{
    unsigned char *data;
    unsigned int i;

    if (frame_size == 0)
        return;

    data = (unsigned char *)malloc(frame_size);
    if (data == NULL)
    {
        fseek(fp, frame_size, SEEK_CUR);
        return;
    }

    if (fread(data, 1, frame_size, fp) != frame_size)
    {
        free(data);
        return;
    }

    printf("Frame ID: WXXX\n");
    printf("Frame Size: %u bytes\n", frame_size);
    printf("URL: ");

    i = 1;

    /* Skip description */
    while (i < frame_size && data[i] != 0)
        i++;

    if (i < frame_size)
        i++;

    while (i < frame_size && data[i] != 0)
    {
        putchar(data[i]);
        i++;
    }

    printf("\n\n");

    free(data);
}

/* ---------------------------------------------------------
   VIEW
   --------------------------------------------------------- */
void view_tag(char *filename)
{
    FILE *fp;
    unsigned char header[10];
    unsigned int tag_size;
    unsigned int bytes_read = 0;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file\n");
        return;
    }

    printf("MP3 file opened successfully\n");

    if (fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR: Invalid MP3 file\n");
        fclose(fp);
        return;
    }

    printf("Header: %c%c%c\n", header[0], header[1], header[2]);

    if (header[0] != 'I' ||
        header[1] != 'D' ||
        header[2] != '3')
    {
        printf("ERROR: Invalid ID3 tag\n");
        fclose(fp);
        return;
    }

    printf("ID3 Version: %d.%d\n", header[3], header[4]);
    printf("Flags: %d\n", header[5]);

    tag_size = get_tag_size(&header[6]);

    printf("Tag size bytes: %d %d %d %d\n",
           header[6], header[7], header[8], header[9]);
    printf("Tag size: %u bytes\n\n", tag_size);

    while (bytes_read + 10 <= tag_size)
    {
        char frame_id[5];
        unsigned char size_bytes[4];
        unsigned char flags[2];
        unsigned int frame_size;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;

        if (frame_id[0] == 0)
            break;

        frame_id[4] = '\0';

        if (fread(size_bytes, 1, 4, fp) != 4)
            break;

        if (fread(flags, 1, 2, fp) != 2)
            break;

        frame_size = get_frame_size(size_bytes);

        if (frame_size > tag_size - bytes_read - 10)
        {
            printf("ERROR: Invalid frame size\n");
            break;
        }

        if (strcmp(frame_id, "TALB") == 0 ||
            strcmp(frame_id, "TPE1") == 0 ||
            strcmp(frame_id, "TIT2") == 0 ||
            strcmp(frame_id, "TYER") == 0 ||
            strcmp(frame_id, "TCON") == 0 ||
            strcmp(frame_id, "TRCK") == 0 ||
            strcmp(frame_id, "TCOP") == 0 ||
            strcmp(frame_id, "TPOS") == 0)
        {
            print_text_frame(fp, frame_id, frame_size);
        }
        else if (strcmp(frame_id, "COMM") == 0)
        {
            print_comment_frame(fp, frame_size);
        }
        else if (strcmp(frame_id, "WXXX") == 0)
        {
            print_wxxx_frame(fp, frame_size);
        }
        else if (strcmp(frame_id, "APIC") == 0)
        {
            printf("Frame ID: APIC\n");
            printf("Frame Size: %u bytes\n", frame_size);
            printf("Album Art: Present\n\n");

            fseek(fp, frame_size, SEEK_CUR);
        }
        else
        {
            printf("Frame ID: %s\n", frame_id);
            printf("Frame Size: %u bytes\n\n", frame_size);
            fseek(fp, frame_size, SEEK_CUR);
        }

        bytes_read += 10 + frame_size;
    }

    fclose(fp);
}

/* ---------------------------------------------------------
   Write edited text into an EXISTING frame.
   Frame header and frame size are NOT changed.
   --------------------------------------------------------- */
static int update_text_frame(FILE *fp,
                             long data_position,
                             unsigned int frame_size,
                             const char *value)
{
    unsigned char *buffer;
    unsigned char encoding;
    unsigned int i;
    unsigned int capacity;

    if (frame_size == 0)
        return 0;

    buffer = (unsigned char *)calloc(frame_size, 1);

    if (buffer == NULL)
        return 0;

    /*
     * Read the original frame payload so that we preserve
     * the original encoding style.
     */
    if (fseek(fp, data_position, SEEK_SET) != 0 ||
        fread(buffer, 1, frame_size, fp) != frame_size)
    {
        free(buffer);
        return 0;
    }

    encoding = buffer[0];

    /* ---------------- Encoding 0 / 3 ---------------- */
    if (encoding == 0 || encoding == 3)
    {
        buffer[0] = encoding;
        capacity = frame_size - 1;

        /* Clear old text first. */
        memset(buffer + 1, 0, capacity);

        for (i = 0; i < capacity && value[i] != '\0'; i++)
            buffer[1 + i] = (unsigned char)value[i];

        /*
         * If the value is too long, it is truncated rather than
         * changing the frame size.
         */
    }
    /* ---------------- UTF-16 ---------------- */
    else
    {
        unsigned int pos = 1;
        unsigned int max_chars;

        /*
         * Preserve BOM if present.
         */
        if (frame_size >= 3 &&
            ((buffer[1] == 0xFF && buffer[2] == 0xFE) ||
             (buffer[1] == 0xFE && buffer[2] == 0xFF)))
        {
            pos = 3;
        }

        /* Clear old text but preserve encoding and BOM. */
        memset(buffer + pos, 0, frame_size - pos);

        /*
         * The supplied MP3 uses UTF-16LE. Write ASCII characters
         * as UTF-16LE while keeping the original frame size.
         */
        max_chars = (frame_size - pos) / 2;

        for (i = 0; i < max_chars && value[i] != '\0'; i++)
        {
            buffer[pos++] = (unsigned char)value[i];
            buffer[pos++] = 0x00;
        }
    }

    if (fseek(fp, data_position, SEEK_SET) != 0 ||
        fwrite(buffer, 1, frame_size, fp) != frame_size)
    {
        free(buffer);
        return 0;
    }

    fflush(fp);
    free(buffer);
    return 1;
}

/* ---------------------------------------------------------
   EDIT
   --------------------------------------------------------- */
void edit_tag(char *filename,
              char *title,
              char *artist,
              char *album,
              char *year,
              char *genre)
{
    FILE *fp;
    unsigned char header[10];
    unsigned int tag_size;
    unsigned int bytes_read = 0;
    int updated = 0;

    fp = fopen(filename, "r+b");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open MP3 file\n");
        return;
    }

    printf("File opened successfully\n");

    if (fread(header, 1, 10, fp) != 10)
    {
        printf("ERROR: Invalid MP3 file\n");
        fclose(fp);
        return;
    }

    if (header[0] != 'I' ||
        header[1] != 'D' ||
        header[2] != '3')
    {
        printf("ERROR: ID3 tag not found\n");
        fclose(fp);
        return;
    }

    if (header[3] != 3)
    {
        printf("ERROR: This editor supports ID3v2.3 only\n");
        fclose(fp);
        return;
    }

    tag_size = get_tag_size(&header[6]);

    printf("ID3v2.3 tag found\n");
    printf("Tag size: %u bytes\n\n", tag_size);

    while (bytes_read + 10 <= tag_size)
    {
        char frame_id[5];
        unsigned char size_bytes[4];
        unsigned char flags[2];
        unsigned int frame_size;
        long data_position;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;

        if (frame_id[0] == 0)
            break;

        frame_id[4] = '\0';

        if (fread(size_bytes, 1, 4, fp) != 4)
            break;

        if (fread(flags, 1, 2, fp) != 2)
            break;

        frame_size = get_frame_size(size_bytes);

        if (frame_size > tag_size - bytes_read - 10)
        {
            printf("ERROR: Invalid frame size for %s\n", frame_id);
            break;
        }

        data_position = ftell(fp);

        printf("Frame ID: %s\n", frame_id);
        printf("Frame Size: %u bytes\n", frame_size);

        if (strcmp(frame_id, "TIT2") == 0)
        {
            printf("Updating Title...\n\n");

            if (update_text_frame(fp, data_position,
                                  frame_size, title))
                updated = 1;
        }
        else if (strcmp(frame_id, "TPE1") == 0)
        {
            printf("Updating Artist...\n\n");

            if (update_text_frame(fp, data_position,
                                  frame_size, artist))
                updated = 1;
        }
        else if (strcmp(frame_id, "TALB") == 0)
        {
            printf("Updating Album...\n\n");

            if (update_text_frame(fp, data_position,
                                  frame_size, album))
                updated = 1;
        }
        else if (strcmp(frame_id, "TYER") == 0)
        {
            printf("Updating Year...\n\n");

            if (update_text_frame(fp, data_position,
                                  frame_size, year))
                updated = 1;
        }
        else if (strcmp(frame_id, "TCON") == 0)
        {
            printf("Updating Genre...\n\n");

            if (update_text_frame(fp, data_position,
                                  frame_size, genre))
                updated = 1;
        }
        else
        {
            /*
             * Do not modify COMM, APIC, WXXX, TCOP, TPOS,
             * TRCK or any other frame.
             */
            printf("\n");

            if (fseek(fp, data_position + frame_size, SEEK_SET) != 0)
                break;

            bytes_read += 10 + frame_size;
            continue;
        }

        if (fseek(fp, data_position + frame_size, SEEK_SET) != 0)
            break;

        bytes_read += 10 + frame_size;
    }

    fclose(fp);

    if (updated)
        printf("Edit completed successfully\n");
    else
        printf("ERROR: No editable frames were updated\n");
}
