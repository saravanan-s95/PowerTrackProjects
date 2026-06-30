#include<stdio.h>
#include<string.h>
#include "view.h"
#include "edit.h"

int main(int argc,char *argv[])
{
    if(argc==2 && strcmp(argv[1],"-h")==0)
    {
        printf("=====================================\n");
        printf("       MP3 Tag Reader Help\n");
        printf("=====================================\n");

        printf("\n./mp3_tag -h : Display Help\n");
        printf("./mp3_tag -v file.mp3 : View MP3 Tags\n");

        printf("./mp3_tag -e -t \"New Title\" file.mp3     : Edit Title\n");
        printf("./mp3_tag -e -a \"New Artist\" file.mp3    : Edit Artist\n");
        printf("./mp3_tag -e -A \"New Album\" file.mp3     : Edit Album\n");
        printf("./mp3_tag -e -y \"2025\" file.mp3          : Edit Year\n");
        printf("./mp3_tag -e -g \"Hindi\" file.mp3         : Edit Genre\n");
        printf("./mp3_tag -e -c \"Comment\" file.mp3       : Edit Comment\n");

        return 0;
    }

    else if(argc==3 && strcmp(argv[1],"-v")==0)
    {
        view_mp3tag(argv[2]);
    }

    else if(argc == 5 && strcmp(argv[1], "-e") == 0)
    {
        edit_tag(argv[2], argv[3], argv[4]);
    }

    else
    {
        printf("Invalid Arguments\n");
        printf("Use ./mp3_tag -h for Help\n");
    }

    return 0;
}