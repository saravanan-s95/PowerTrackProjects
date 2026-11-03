#include <stdio.h>
#include "display.h"

void display_tags(MP3Tag *tag)
{
    printf("\n");
    printf("========================================\n");
    printf("          MP3 TAG READER\n");
    printf("========================================\n");

    printf("%-12s: %s\n", "Title", tag->title);
    printf("%-12s: %s\n", "Artist", tag->artist);
    printf("%-12s: %s\n", "Album", tag->album);
    printf("%-12s: %s\n", "Year", tag->year);
    printf("%-12s: %s\n", "Genre", tag->genre);
    printf("%-12s: %s\n", "Track", tag->track);
    printf("%-12s: %s\n", "Comment", tag->comment);

    printf("========================================\n\n");
}