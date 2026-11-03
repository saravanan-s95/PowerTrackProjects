
#include <stdio.h>
#include <string.h>
#include "mp3_tag_reader.h"

int main(int argc, char *argv[])
{
    int ret;

    if(argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        printf("Help menu for Mp3 Tag Reader and Editor:\n");
        printf("./mp3_tag_reader -v <.mp3 file> For viewing the tags\n");
        printf("For editing the tags:\n");
        printf("./mp3_tag_reader -e <.mp3 file> <modifier> <tag>\n");
        printf("Modifier\tFunction\n");
        printf("-t\t\tModifies Title tag\n");
        printf("-T\t\tModifies Track tag\n");
        printf("-a\t\tModifies Artist tag\n");
        printf("-A\t\tModifies Album tag\n");
        printf("-y\t\tModifies Year tag\n");
        printf("-C\t\tModifies Comment tag\n");
        printf("-g\t\tModifies Genre tag\n");
        return SUCCESS;
    }
    else if(argc == 3 && strcmp(argv[1], "-v") == 0)
    {
        ret = view_tags(argv[2]);
    }
    else if(argc == 5 && strcmp(argv[1], "-e") == 0)
    {
        if(strcmp(argv[3], "-t") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TIT2");
        }
        else if(strcmp(argv[3], "-T") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TRCK");
        }
        else if(strcmp(argv[3], "-a") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TPE1");
        }
        else if(strcmp(argv[3], "-A") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TALB");
        }
        else if(strcmp(argv[3], "-y") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TYER");
        }
        else if(strcmp(argv[3], "-C") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "COMM");
        }
        else if(strcmp(argv[3], "-g") == 0)
        {
            ret = edit_tag(argv[4], argv[2], "TCON");
        }
        else
        {
            printf("Invalid tag option\n");
            return FAILURE;
        }
    }
    else
    {
        printf("Error: ./mp3_tag_reader: Invalid Arguments\n");
        printf("Usage: ./mp3_tag_reader -h for help\n");
        return FAILURE;
    }

    return ret;
}
