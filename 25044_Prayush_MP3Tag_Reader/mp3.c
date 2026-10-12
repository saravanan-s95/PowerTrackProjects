#include "helpers.h"
#include "mp3.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   HEADER READING
   ============================================================ */

Status readHeaders(Header *header, FILE *file) {
    if (!header || !file)
        return failure;

    rewind(file);

    unsigned char buffer[10];

    if (fread(buffer, 10, 1, file) != 1)
        return failure;

    if (memcmp(buffer, "ID3", 3) != 0)
        return failure;

    if (buffer[3] != 3 && buffer[3] != 4)
        return failure;

    memcpy(header->magic, buffer, 3);
    header->magic[3] = '\0';

    header->version = buffer[3];
    header->revision = buffer[4];
    header->flags = buffer[5];

    readSynchSafeIntFromBuffer(&header->size, buffer + 6);

    return success;
}

/* ============================================================
   FRAME READING
   ============================================================ */

Status readFrame(Frame *frame, FILE *file, unsigned char version, long tagEnd) {

    if (!frame || !file)
        return failure;

    unsigned char headerBuffer[10];

    if (fread(headerBuffer, 1, 10, file) != 10)
        return failure;

    /* Padding reached */
    if (headerBuffer[0] == 0x00)
        return failure;

    memcpy(frame->tag, headerBuffer, 4);
    frame->tag[4] = '\0';

    if (version == 3)
        readIntFromBuffer(&frame->size, headerBuffer + 4);
    else
        readSynchSafeIntFromBuffer(&frame->size, headerBuffer + 4);

    /* Validate frame size */
    if (frame->size == 0 || frame->size > (unsigned int)(tagEnd - ftell(file)))
        return failure;

    /* Skip flags (2 bytes) */
    /* headerBuffer[8] and headerBuffer[9] */

    unsigned char *contentBuffer = malloc(frame->size);
    if (!contentBuffer)
        return failure;

    if (fread(contentBuffer, 1, frame->size, file) != frame->size) {
        free(contentBuffer);
        return failure;
    }

    /* Skip text encoding byte (first byte) */
    unsigned int copySize = (frame->size > 1) ? frame->size - 1 : 0;

    if (copySize > 99)
        copySize = 99;

    memcpy(frame->content, contentBuffer + 1, copySize);
    frame->content[copySize] = '\0';

    free(contentBuffer);
    return success;
}

/* ============================================================
   METADATA READING
   ============================================================ */

Status readMetadata(Metadata *metadata, FILE *file) {

    if (readHeaders(&metadata->header, file) == failure)
        return failure;

    long tagStart = ftell(file);
    long tagEnd = tagStart + metadata->header.size;

    int remainingTags = 6;
    while (ftell(file) < tagEnd) {

        if (tagEnd - ftell(file) < 10)
            break;

        Frame frame;

        if (readFrame(&frame, file, metadata->header.version, tagEnd) ==
            failure)
            break;

        if (strcmp(frame.tag, "TIT2") == 0) {
            remainingTags--;
            metadata->title = frame;
        }

        else if (strcmp(frame.tag, "TPE1") == 0) {
            remainingTags--;
            metadata->artist = frame;
        }

        else if (strcmp(frame.tag, "TALB") == 0) {
            remainingTags--;
            metadata->album = frame;
        }

        else if (strcmp(frame.tag, "TYER") == 0) {
            remainingTags--;
            metadata->year = frame;
        }

        else if (strcmp(frame.tag, "TCON") == 0) {
            remainingTags--;
            metadata->content = frame;
        }

        else if (strcmp(frame.tag, "COMM") == 0) {
            remainingTags--;
            metadata->composer = frame;
        }

        if (remainingTags == 0)
            break;
    }

    return success;
}

/* ============================================================
   HEADER WRITING
   ============================================================ */

void writeHeaders(const Header *header, FILE *file) {

    fwrite(header->magic, 3, 1, file);
    fwrite(&header->version, 1, 1, file);
    fwrite(&header->revision, 1, 1, file);
    fwrite(&header->flags, 1, 1, file);

    unsigned char sizeBuffer[4];
    writeSynchSafeIntToBuffer(&header->size, sizeBuffer);

    fwrite(sizeBuffer, 4, 1, file);
}

/* ============================================================
   FRAME EDITING
   ============================================================ */

Status writeFrameToFile(const char *targetTag, FILE *source,
                        const char *newContent, FILE *dest, long tagEnd,
                        unsigned char version, int *sizeDiff) {

    unsigned char headerBuffer[10];
    int found = 0;

    while (ftell(source) < tagEnd) {

        if (fread(headerBuffer, 10, 1, source) != 1)
            return failure;

        /* Proper padding detection: all 4 ID bytes zero */
        if (headerBuffer[0] == 0x00 && headerBuffer[1] == 0x00 &&
            headerBuffer[2] == 0x00 && headerBuffer[3] == 0x00) {

            /* rewind and copy remaining padding */
            fseek(source, -10, SEEK_CUR);

            while (ftell(source) < tagEnd)
                fputc(fgetc(source), dest);

            return found ? success : failure;
        }

        char currentTag[5];
        memcpy(currentTag, headerBuffer, 4);
        currentTag[4] = '\0';

        unsigned int frameSize;

        if (version == 3)
            readIntFromBuffer(&frameSize, headerBuffer + 4);
        else
            readSynchSafeIntFromBuffer(&frameSize, headerBuffer + 4);

        if (frameSize == 0 ||
            frameSize > (unsigned int)(tagEnd - ftell(source)))
            return failure;

        unsigned char *contentBuffer = malloc(frameSize);
        if (!contentBuffer)
            return failure;

        if (fread(contentBuffer, frameSize, 1, source) != 1) {
            free(contentBuffer);
            return failure;
        }

        if (strcmp(currentTag, targetTag) == 0) {

            found = 1;

            unsigned int newSize = strlen(newContent) + 1;

            /* Update size in headerBuffer directly */
            if (version == 3)
                writeIntToBuffer(&newSize, headerBuffer + 4);
            else
                writeSynchSafeIntToBuffer(&newSize, headerBuffer + 4);

            fwrite(headerBuffer, 1, 10, dest);

            /* Encoding byte */
            fputc(0x00, dest);

            if (fwrite(newContent, strlen(newContent), 1, dest) != 1)
                return failure;

            // update sizeDiff
            *sizeDiff = newSize - frameSize;
        } else {
            /* Copy frame unchanged */
            fwrite(headerBuffer, 10, 1, dest);
            fwrite(contentBuffer, frameSize, 1, dest);
        }

        free(contentBuffer);
    }

    return found ? success : failure;
}
/* ============================================================
   HIGH LEVEL EDIT
   ============================================================ */
Status editFrame(const char *tag, MP3_info *info) {

    FILE *file = fopen(info->file_name, "rb");

    info->temp_file_name = "./Assets/temp.mp3";
    FILE *temp = fopen(info->temp_file_name, "wb");
    if (!temp)
        return failure;

    Header header;

    if (readHeaders(&header, file) == failure) {
        fclose(file);
        fclose(temp);
        return failure;
    }

    long tagStart = ftell(file);
    long tagEnd = tagStart + header.size;

    writeHeaders(&header, temp);

    int sizeDiff;
    if (writeFrameToFile(tag, file, info->content, temp, tagEnd, header.version,
                         &sizeDiff) == failure) {
        fclose(temp);
        return failure;
    }

    int ch;
    while ((ch = fgetc(file)) != EOF)
        fputc(ch, temp);

    // update header size
    fseek(temp, 6, SEEK_SET);
    unsigned int newSize = header.size + sizeDiff;
    unsigned char sizeBuffer[4];

    writeSynchSafeIntToBuffer(&newSize, sizeBuffer);

    fwrite(sizeBuffer, sizeof(char), 4, temp);

    fclose(file);
    fclose(temp);
    return success;
}
