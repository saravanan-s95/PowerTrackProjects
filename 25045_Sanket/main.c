#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "reader.h"
#include "writer.h"
#include "error_handling.h"

static void print_banner(void)
{
    printf("\n");
    printf("=========================================\n");
    printf("      MP3 TAG READER & EDITOR v2.0\n");
    printf("=========================================\n");
}

void display_help(void)
{
    print_banner();

    printf("\nUsage:\n");
    printf("  ./mp3_tag_editor [option] [arguments]\n\n");

    printf("View:\n");
    printf("  -v  <file>\n");
    printf("  --view <file>\n\n");

    printf("Edit:\n");
    printf("  -e <tag> <value> <file>\n");
    printf("  --edit <tag> <value> <file>\n\n");

    printf("Delete Tag:\n");
    printf("  -d <file> <tag>\n");
    printf("  --delete <file> <tag>\n\n");

    printf("Delete All Tags:\n");
    printf("  -D <file>\n");
    printf("  --delete-all <file>\n\n");

    printf("Extract Album Art:\n");
    printf("  -x <file> <image>\n");
    printf("  --extract <file> <image>\n\n");

    printf("Supported Tags:\n");
    printf("  -t  Title\n");
    printf("  -a  Artist\n");
    printf("  -A  Album\n");
    printf("  -y  Year\n");
    printf("  -c  Comment\n");
    printf("  -g  Genre\n");
    printf("  -T  Track\n");
    printf("  -b  Album Artist\n");
    printf("  -m  Composer\n");
    printf("  -o  Copyright\n");
    printf("  -p  Publisher\n\n");

    printf("Examples:\n");
    printf("  ./mp3_tag_editor -v song.mp3\n");
    printf("  ./mp3_tag_editor -e -t \"New Title\" song.mp3\n");
    printf("  ./mp3_tag_editor -d song.mp3 -t\n");
    printf("  ./mp3_tag_editor -D song.mp3\n");
    printf("  ./mp3_tag_editor -x song.mp3 cover.jpg\n");
}

static int handle_view(int argc, char *argv[])
{
    if(argc != 3)
        return 1;

    view_tags(argv[2]);

    return 0;
}

static int handle_edit(int argc, char *argv[])
{
    if(argc != 5)
        return 1;

    return edit_tag(
            argv[4],
            argv[2],
            argv[3]);
}

static int handle_delete(int argc, char *argv[])
{
    if(argc != 4)
        return 1;

    return delete_tag(
            argv[2],
            argv[3]);
}

static int handle_delete_all(int argc, char *argv[])
{
    if(argc != 3)
        return 1;

    return delete_all_tags(argv[2]);
}

static int handle_extract(int argc, char *argv[])
{
    if(argc != 4)
        return 1;

    return extract_album_art(
            argv[2],
            argv[3]);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        display_help();
        return EXIT_FAILURE;
    }

    if(strcmp(argv[1], "-h") == 0 ||
       strcmp(argv[1], "--help") == 0)
    {
        display_help();
        return EXIT_SUCCESS;
    }

    if(strcmp(argv[1], "-v") == 0 ||
       strcmp(argv[1], "--view") == 0)
    {
        return handle_view(argc, argv);
    }

    if(strcmp(argv[1], "-e") == 0 ||
       strcmp(argv[1], "--edit") == 0)
    {
        return handle_edit(argc, argv);
    }

    if(strcmp(argv[1], "-d") == 0 ||
       strcmp(argv[1], "--delete") == 0)
    {
        return handle_delete(argc, argv);
    }

    if(strcmp(argv[1], "-D") == 0 ||
       strcmp(argv[1], "--delete-all") == 0)
    {
        return handle_delete_all(argc, argv);
    }

    if(strcmp(argv[1], "-x") == 0 ||
       strcmp(argv[1], "--extract") == 0)
    {
        return handle_extract(argc, argv);
    }

    display_error("Unknown command");

    display_help();

    return EXIT_FAILURE;
}