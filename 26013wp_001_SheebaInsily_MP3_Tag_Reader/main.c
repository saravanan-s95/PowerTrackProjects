
/*
 * Project Name : MP3 Tag Reader and Editor
 * Description  : Reads and edits ID3 metadata tags in MP3 files.
 *                Supports viewing existing tags and modifying
 *                tag information such as title, artist, album,
 *                year, genre, and comments.
 * Author       : Sheeba Insily R
 * Batch        : 26013
 * Language     : C
 * File         : main.c
 */

 #include <stdio.h>
#include "mp3_tag_reader.h"

int main(int argc, char *argv[])
{
    int operation;

    if (argc < 2)
    {
        printf("ERROR: Insufficient arguments\n");
        return 0;
    }

    operation = check_operation_type(argv);

    if (operation == 1)
    {
        if (argc != 3)
        {
            printf("ERROR: Usage: ./a.out -v <mp3file>\n");
            return 0;
        }

        view_tag(argv[2]);
    }
    else if (operation == 2)
    {
        if (argc != 13)
        {
            printf("ERROR: Usage: ./a.out -e <mp3file> -t <title> -a <artist> -A <album> -y <year> -g <genre>\n");
            return 0;
        }

        if (argv[3][0] != '-' || argv[5][0] != '-' ||
            argv[7][0] != '-' || argv[9][0] != '-' ||
            argv[11][0] != '-')
        {
            printf("ERROR: Invalid edit options\n");
            return 0;
        }

        if (argv[3][1] != 't' || argv[5][1] != 'a' ||
            argv[7][1] != 'A' || argv[9][1] != 'y' ||
            argv[11][1] != 'g')
        {
            printf("ERROR: Invalid edit options\n");
            return 0;
        }

        edit_tag(argv[2],
                 argv[4],
                 argv[6],
                 argv[8],
                 argv[10],
                 argv[12]);
    }
    else if (operation == 3)
    {
        printf("\nMP3 Tag Reader and Editor\n\n");
        printf("View : ./a.out -v <mp3file>\n");
        printf("Edit : ./a.out -e <mp3file> -t <title> -a <artist> -A <album> -y <year> -g <genre>\n");
    }
    else
    {
        printf("ERROR: Unsupported operation\n");
    }

    return 0;
}
