#include "helpers.h"
#include "logs.h"
#include "mp3.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

Status read_and_validate_editMP3_args(char *argv[], MP3_info *info) {
    // validate file
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

    // validate tag flags
    switch (argv[3][1]) {
    case 't':
        info->tag_to_edit = 't';
        break;
    case 'a':
        info->tag_to_edit = 'a';
        break;
    case 'A':
        info->tag_to_edit = 'A';
        break;
    case 'y':
        info->tag_to_edit = 'y';
        break;
    case 'c':
        info->tag_to_edit = 'c';
        break;
    case 'C':
        info->tag_to_edit = 'C';
        break;
    default:
        LOG_ERROR(ERR_INVALID_TAG);
        return failure;
    }

    info->content = argv[4];
    fclose(fptr_mp3_file);
    return success;
}

Status copy_temp_to_original(MP3_info *info) {
    FILE *fptr_original = fopen(info->file_name, "wb");
    FILE *fptr_temp = fopen(info->temp_file_name, "rb");

    int ch;
    while ((ch = fgetc(fptr_temp)) != EOF)
        fputc(ch, fptr_original);

    fclose(fptr_temp);
    fclose(fptr_original);
    return success;
}

Status editTitle(MP3_info *info) {
    if (editFrame("TIT2", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editArtist(MP3_info *info) {
    if (editFrame("TPE1", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editAlbum(MP3_info *info) {
    if (editFrame("TALB", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editYear(MP3_info *info) {
    if (editFrame("TYER", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editContentType(MP3_info *info) {
    if (editFrame("TCON", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editComposer(MP3_info *info) {
    if (editFrame("COMM", info) == failure)
        return failure;
    if (copy_temp_to_original(info) == failure)
        return failure;
    return success;
}

Status editTags(MP3_info *info) {
    switch (info->tag_to_edit) {
    case 't':
        return editTitle(info);
    case 'a':
        return editArtist(info);
    case 'A':
        return editAlbum(info);
    case 'y':
        return editYear(info);
    case 'c':
        return editContentType(info);
    case 'C':
        return editComposer(info);
    default:
        return failure;
    }
}
