/*

    Name : Anjana V
    Description : MP3 TAG reader/  editor using ID3V2.3 version
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main(int argc, char *argv[])
{
    MP3Info mp3;
    /* check ./a.out <arg> present */
    if(argc > 1)
    {
        // copy view/ edit/ help option from CLA to opt
        char *opt = argv[1];
        /* VIEW option */
        if(strcmp(opt, "-v") == 0 && argc == 3)
        {
            printf("view 1\n");
            //char *filename = argv[2];
            mp3.filename = argv[2]; 
            if(validate_mp3(&mp3) == SUCCESS)
            {
                printf("view 2\n");
                view_tags(&mp3);
            }
            /* Invalid arguments */
            else
            {
                mp3_error();
            }
        }
        /* EDIT option */
        else if(strcmp(opt, "-e") == 0 && argc == 5)
        {
            printf("edit 1 \n");
            /* 
            checking editing options 
                -t -> to edit song title
                -a -> to edit artist name
                -A -> to edit album name
                -y -> to edit year
                -m -> to edit content
                -c -> to edit comment
            */
            char *edit = argv[2];
            char *new_data = argv[3];
            if(strcmp(edit, "-t") == 0 || strcmp(edit, "-a") == 0 || strcmp(edit, "-A") == 0 || strcmp(edit, "-m") == 0 || strcmp(edit, "-y") == 0 || strcmp(edit, "-c") == 0)
            {
                printf("edit 2\n");
                mp3.filename = argv[4]; 
                if(validate_mp3(&mp3) == SUCCESS)
                {
                    printf("edit 3\n");
                    switch(argv[2][1])
                    {
                        case 't':
                            edit_title(&mp3, new_data);
                            break;

                        case 'a':
                            edit_artist(&mp3, new_data);
                            break;

                        case 'A':
                            edit_album(&mp3, new_data);
                            break;

                        case 'y':
                            edit_year(&mp3, new_data);
                            break;

                        case 'm':
                            edit_genre(&mp3, new_data);
                            break;

                        case 'c':
                            edit_comment(&mp3, new_data);
                            break;

                        default:
                            mp3_error();
                    }
                }
                /* Invalid arguments */
                else
                {
                    mp3_error();
                    return FAILURE;
                }
            }
            /* Invalid arguments */
            else
            {
                mp3_error();
                return FAILURE;
            }
        }
        /* HELP option */
        else if(strcmp(opt, "--help") == 0)
        {
            printf("help\n");   
        }
        /* Invalid arguments */
        else
        {
            mp3_error();
            return FAILURE;
        }
        return SUCCESS;
    }
    /* Invalid arguments */
    else
    {
        mp3_error();
        return FAILURE;
    }
    return SUCCESS;
}