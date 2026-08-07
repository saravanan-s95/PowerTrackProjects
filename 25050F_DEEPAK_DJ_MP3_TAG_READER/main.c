/**
 * @file main.c
 * @brief Main entry point for the MP3 Tag Reader application.
 */

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "id3_reader.h"
#include "id3_writer.h"
#include "error_handling.h"

/**
 * @brief Displays the help message for the MP3 Tag Reader application.
 */
 void display_error(const char* message){
     printf("\t%s\n",message);
 }
void display_arg_error() {
    printf("---------------------------ERROR--------------------------------------\n");
    printf("ERROR: ./a.out : Invalid arguments\n");
    printf("Usage: \n");
    printf("Options:\n");
    printf("To view please pass like : ./a.out -v mp3file\n");
    printf("To edit please pass like : ./a.out -e -t/-a/-A/-g/-y/-c \"changing text\" mp3file\n");
    printf("To download the image please pass like : ./a.out -i <image name> mp3file\n");
    printf("To get help please pass like : ./a.out -help\n");
    printf("----------------------------------------------------------------------\n");
}
void display_help(){
     printf("-----------------------HELP MENU--------------------------------\n");
     printf("\t-v -> to view mp3 file contemt\n");
     printf("\t-e -> to edit mp3 file content\n");
     printf("\t-i -> to get the image from mp3 file\n");
     printf("\t2.1 -t -> to edit song title\n");
     printf("\t2.2 -a -> to edit artist name\n");
     printf("\t2.3 -A -> to edit Album name\n");
     printf("\t2.4 -g -> to edit genre\n");
     printf("\t2.5 -y -> to edit to edit year\n");
     printf("\t2.6 -c -> to edit composer\n");
     printf("-----------------------------------------------------------------\n");
}
/**
 * @brief Main function to handle command-line arguments and execute appropriate actions.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, non-zero on failure.
 */
int check_tag_cla(char *tag)
{
   if(!strcmp(tag,"-t")) return 1;
   else if(!strcmp(tag,"-a")) return 1;
   else if(!strcmp(tag,"-A")) return 1;
   else if(!strcmp(tag,"-y")) return 1;
   else if(!strcmp(tag,"-g")) return 1;
   else if(!strcmp(tag,"-c")) return 1;
   else return 0;
}
int download_image(char *image_name,char *filename);
int main(int argc, char *argv[]) {
    if (argc < 2) {
        display_arg_error();
        return 1;
    }

    if (strcmp(argv[1], "-help") == 0){
        display_help();
        return 0;
    } 
    else if (strcmp(argv[1], "-v") == 0 && argc == 3)
    {
        if(!strstr(argv[2],".mp3")){
            display_error("Please provide a valid mp3 file.");
            return 0;
        }
                view_tags(argv[2]);

    } 
    else if (strcmp(argv[1], "-e") == 0 && argc == 5) 
    {
        char *tag = argv[2];
        char *filename = argv[4];
        char *value = argv[3];
        if(!check_tag_cla(tag))
        {
            printf("\n\t----------------Enter valid CLA----------\n\n");
            display_help();
            return 0;
        }
        if(!strstr(filename,".mp3"))
        {
            display_error("ENTER VALID FILE NAME PRESENT IN DIRECTORY");
            display_arg_error();
            return 0;
        }
        if (edit_tag(filename, tag, value) == 0) 
        {
            display_error("Failed to edit tag.");
            return 0;
        }
        printf("\tTag edited successfully.\n");
    }
    else if(strcmp(argv[1],"-i")==0 && argc == 4)
    {
        char *image_name=argv[2];
        char *filename = argv[3];
         if(!strstr(filename,".mp3"))
        {
            display_error("ENTER VALID FILE NAME PRESENT IN DIRECTORY");
            display_arg_error();
            return 0;
        }
        if(!download_image(image_name,filename)){
             display_error("Failed to download image.");
            return 0;
        }else{
            printf("\tIMAGE DOWNLOADED SUCCESSFULLY\n");
        }
    }
    else 
    {
         display_arg_error();
        return 0;
    }

    return 0;
}
