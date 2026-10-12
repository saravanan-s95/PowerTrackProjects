#include "helpers.h"
#include "logs.h"
#include "mp3.h"
#include "render.h"
#include "types.h"
#include "view.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

Status read_and_validate_viewMP3_args(char *argv[], MP3_info *info) {
    // validate mp3 file
    char *file_extn = strstr(argv[2], ".mp3");
    if (!file_extn || strcmp(file_extn, ".mp3")) {
        LOG_ERROR(ERR_INVALID_FILE);
        return failure;
    }

    info->file_name = argv[2];

    FILE *fptr_mp3_file = fopen(info->file_name, "rb");

    char magic[4];
    fread(magic, sizeof(char) * 3, 1, fptr_mp3_file);
    magic[3] = '\0';

    if (strcmp(magic, MP3_MAGIC)) {
        fclose(fptr_mp3_file);
        LOG_ERROR(ERR_INVALID_FILE);
        return failure;
    }
    return success;
}

void printMetadata(Metadata *metadata) {
    printTableHeaders();

    displayFrame(&metadata->title);
    displayFrame(&metadata->artist);
    displayFrame(&metadata->album);
    displayFrame(&metadata->year);
    displayFrame(&metadata->composer);
    displayFrame(&metadata->content);
}

Status viewTags(MP3_info *info) {
    FILE *file = fopen(info->file_name, "rb");

    if (!file) {
        printf("Error in opening file");
        return failure;
    }

    Metadata metadata;
    metadata.tagCount = 6;

    readMetadata(&metadata, file);
    printMetadata(&metadata);

    fclose(file);
    return success;
}
