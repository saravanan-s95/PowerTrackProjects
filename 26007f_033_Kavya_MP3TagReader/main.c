/* Name: Kavya.M
   Date: 12.09.2026
   Description: To read the MP3 tag information from the mp3 file ID3 tag to display it and edit it 
   Operations: Help View Edit (inputs through cmd line argument)
   1. Read ID3 tag informations from the file and display the Title Artist Album Year Content Comment
   2. Edit the ID3 tag informations and save them back to the file 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mp3view(char *filename);
int mp3edit(char *filename, char *option, char *new_value);

int main(int argc, char *argv[])
{
    char header[3]; // to store the header of the ID3 tag
    unsigned char version[2]; // to store the version of the ID3 tag
    unsigned char flags; // to store the flags of the ID3 tag

    if(argc < 2) // only ./a.out is given
    {
        printf("ERROR: ./a.out is INVALID ARGUMENT\n");
        printf("USAGE:\n");
        printf("To View the ID3 tag information: ./a.out -v <mp3_file>\n");
        printf("To Edit the ID3 tag information: ./a.out -e -t/-a/-A/-y/-m/-c new_name <mp3_file>\n");
        printf("To Get Help: ./a.out --help\n");
        return 0;
    }
    if(argc == 2) // ./a.out --help
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            printf("------------------------HELP MENU---------------------------\n");
            printf("1. -v : View MP3 tag information\n");
            printf("2. -e : Edit MP3 tag information\n");
            printf("\t2.1. -t : Edit Title Name\n");
            printf("\t2.2. -a : Edit Artist Name\n");
            printf("\t2.3. -A : Edit Album Name\n");
            printf("\t2.4. -y : Edit Year\n");
            printf("\t2.5. -m : Edit Content\n");
            printf("\t2.6. -c : Edit Comment\n");
            printf("----------------------------------------------------------------\n");
        }
        else
        {
            printf("ERROR: INVALID ARGUMENT\n");
        }
        return 0;
    }
    if(argc == 3) // ./a.out -v sample.mp3
    {
        if(strcmp(argv[1], "-v") != 0)
        {
            printf("ERROR: Invalid View Command\n");
            return 0;
        }

        if(strstr(argv[2], ".mp3") == NULL) // .mp3 extension checked
        {
            printf("ERROR: File is not an MP3 file\n");
            return 0;
        }

        FILE *fp = fopen(argv[2], "rb"); // open file

        if(fp == NULL)
        {
            printf("ERROR: File Not Found\n");
            return 0;
        }

        printf("File Found\n");

        if(fread(header, 1, 3, fp) != 3) // check if ID3 header is present or not
        {
            printf("ERROR: Unable to read ID3 header\n");
            fclose(fp);
            return 0;
        }
        if(header[0] != 'I' || header[1] != 'D' || header[2] != '3') // check if ID3 header is present or not
        {
            printf("ERROR: ID3 tag not found\n");
            fclose(fp);
            return 0;
        }
        
        printf("ID3 tag present\n");

        if(fread(version, 1, 2, fp) != 2) // check if ID3 version is present or not
        {
            printf("ERROR: Unable to read ID3 version\n");
            fclose(fp);
            return 0;
        }

        if(version[0] != 3 || version[1] != 0)
        {
            printf("ERROR: Unsupported ID3 version\n");
            fclose(fp);
            return 0;
        }

        printf("ID3 version is ID3v2.3.0\n");

        fread(&flags, 1, 1, fp); // read the flags of the ID3 tag

        fclose(fp);
        mp3view(argv[2]); // valid function call to view the ID3 tag information

        return 0;
    }
    if(argc == 5) // ./a.out -e -t/-a/-A/-y/-m/-c sample.mp3
    {
        if(strcmp(argv[1], "-e") != 0) // check cmd line is -e
        {
            printf("ERROR: Invalid Edit Command\n");
            return 0;
        }

        if(strcmp(argv[2], "-t") != 0 && strcmp(argv[2], "-a") != 0 && strcmp(argv[2], "-A") != 0 && strcmp(argv[2], "-y") != 0 && strcmp(argv[2], "-m") != 0 && strcmp(argv[2], "-c") != 0)
        {
            printf("ERROR: Invalid Edit Option\n");
            return 0;
        }

        if(strstr(argv[4], ".mp3") == NULL) // check .mp3 extension
        {
            printf("ERROR: File is not an MP3 file\n");
            return 0;
        }

        FILE *fp = fopen(argv[4], "rb"); // open file in read mode

        if(fp == NULL)
        {
            printf("ERROR: File Not Found\n");
            return 0;
        }

        printf("File Found\n");

        if(fread(header, 1, 3, fp) != 3) // check if ID3 header is present or not
        {
            printf("ERROR: Unable to read ID3 header\n");
            fclose(fp);
            return 0;
        }

        if(header[0] != 'I' || header[1] != 'D' || header[2] != '3') // check if ID3 header is present or not
        {
            printf("ERROR: ID3 tag not found\n");
            fclose(fp);
            return 0;
        }

        printf("ID3 tag present\n");

        if(fread(version, 1, 2, fp) != 2) // check if ID3 version is present or not
        {
            printf("ERROR: Unable to read ID3 version\n");
            fclose(fp);
            return 0;
        }

        if(version[0] != 3 || version[1] != 0) 
        {
            printf("ERROR: Unsupported ID3 version\n");
            fclose(fp);
            return 0;
        }

        printf("ID3 version is ID3v2.3.0\n");

        fread(&flags, 1, 1, fp); // read the flags of the ID3 tag

        fclose(fp);

        mp3edit(argv[4], argv[2], argv[3]); // valid function call to edit the ID3 tag information

        return 0;
    }
    printf("ERROR: Invalid Number of Arguments\n");

    return 0;
}

