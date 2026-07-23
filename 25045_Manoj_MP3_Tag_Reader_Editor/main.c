#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "view.h"
#include "edit.h"
#include "types.h"

OperationType check_operation_type(char *argv[]){
    if(strcmp(argv[1] , "-v") == 0)
        return view_mp3_file_data;
    else if(strcmp(argv[1] , "-e") == 0)
        return edit_mp3_file_data;
    else
        return unsupported;
}

int main(int argc , char *argv[]){
    if(argc == 1){
        printf("\nWELCOME TO MP3 TAG READER AND EDITOR\n");
        printf("=======================================\n\n");
        printf("1. To view tags from an MP3 file use:\n    ./a.out -v <mp3_filename>\n\n");
        printf("2. To edit tags in an MP3 file use:\n    ./a.out -e -‹option> <new text> <mp3 filename>\n\n");
        printf("3. For more details, use:\n    ./a.out --help\n");
        return 1;
    }

    if(argc > 1 && (strcmp(argv[1] , "--help") == 0)){
        printf("\n--------------------MP3 TAG READER AND EDITOR HELP--------------------\n\n");
        printf("1. To view tags from an MP3 file use:\n    ./a.out -v <mp3_filename>\n\n");
        printf("2. To edit tags in an MP3 file use:\n    ./a.out -e -‹option> <new text> <mp3 filename>\n\n");
        printf("Edit options:\n");
        printf("  %-3s  %s\n", "-t", "edit song title");
        printf("  %-3s  %s\n", "-a", "edit artist name");
        printf("  %-3s  %s\n", "-A", "edit album name");
        printf("  %-3s  %s\n", "-y", "edit year");
        printf("  %-3s  %s\n", "-m", "edit genre");
        printf("  %-3s  %s\n", "-c", "edit comment");
    }
    
    ViewInfo view;
    EditInfo edit;
    int return_value = check_operation_type(argv);
    if(return_value == view_mp3_file_data){
        if(argc == 2 || argc > 3){
            printf("Invalid arguments\n");
            printf("To view tags from an MP3 file use:\n    ./a.out -v <mp3_filename>\n");
            return 1;
        }
        if((read_and_validate_view_args(argv , &view)) == e_success){
            if((view_mp3_meta_data(&view)) != e_failure){
                printf("\n\n-------------------SELECTED VIEW DETAILS-------------------\n\n");
                display_frames(&view);
                printf("\n-------------------DETAILS DISPLAYED SUCCESSFULLY-------------------\n\n");
            }
        }else{
            printf("Failed to view mp3 file\n");
        }
    }else if (return_value == edit_mp3_file_data){
        if(argc < 5 || argc > 5){
            printf("Invalid arguments\n");
            printf("To edit tags from an MP3 file:\n    ./a.out -e -‹option> <new text> <mp3 filename>\n");
            return 1;
        }
        if(read_and_validate_edit_args(argv , &edit) != e_failure){
            if(edit_mp3_meta_data(&edit) == e_success){
                 for(int i = 0; i <= 100; i++){
                    printf("[Please wait.......%d%%]\r", i);
                    fflush(stdout);
                    if (i < 90)
                        usleep(50000);
                    else
                        usleep(300000);
                }
                printf("                            \r");
                printf("Edited Successfully");
                printf("\n\n-------------------SELECTED EDIT DETAILS-------------------\n\n");
                memset(&view, 0, sizeof(ViewInfo));
                view.mp3_file_name = edit.mp3_file_name;
                if(view_mp3_meta_data(&view) == e_success)
                        display_frames(&view);
                printf("\n-------------------DETAILS DISPLAYED SUCCESSFULLY-------------------\n\n");
            }else{
                 printf("Failed to edit mp3 file\n");
            }
        }
    }
}


