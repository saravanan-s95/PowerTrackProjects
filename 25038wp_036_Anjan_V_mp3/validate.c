#include "main.h"
/*
int validate_mp3(MP3Info *mp3)
{
    char *ext;

    ext = strrchr(mp3->filename, '.');

    if(ext == NULL)
        return FAILURE;

    if(strcmp(ext, ".mp3") != 0)
        return FAILURE;

    mp3->fptr = fopen(mp3->filename, "rb");

    if(mp3->fptr == NULL)
        return FAILURE;

    char tag[4];

    fread(tag, 1, 3, mp3->fptr);
    tag[3] = '\0';

    fclose(mp3->fptr);

    if(strcmp(tag, "ID3") != 0)
        return FAILURE;

    return SUCCESS;
}
*/
int validate_mp3(MP3Info *mp3)
{
    char *ext = strrchr(mp3->filename, '.');

    if (ext == NULL || strcmp(ext, ".mp3") != 0)
        return FAILURE;

    mp3->fptr = fopen(mp3->filename, "rb");
    if (mp3->fptr == NULL)
        return FAILURE;

    char tag[4];
    fread(tag, 1, 3, mp3->fptr);
    tag[3] = '\0';

    if (strcmp(tag, "ID3") != 0)
    {
        fclose(mp3->fptr);
        return FAILURE;
    }

    unsigned char version[2];
    fread(version, 1, 2, mp3->fptr);

    fclose(mp3->fptr);

    if (version[0] != 3 || version[1] != 0)
        return FAILURE;

    return SUCCESS;
}

void mp3_error()
{
    printf("\n-----------------------------------------------------------------------------------------------------\n\n");
    printf("ERROR:\n\n./a.out: INVALID ARGUMENTS\n\nUSAGE:\nTo view please pass like: ./a.out -v mp3filename\nTo edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\nTo get help pass like: ./a.out --help\n");
    printf("\n-----------------------------------------------------------------------------------------------------\n");
}