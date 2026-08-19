#include<stdio.h>
#include<string.h>
#include "view.h"

void view_mp3tag(char *filename)
{
    FILE *fp = fopen(filename,"rb");

    if(fp == NULL)
    {
        printf("Error : Unable to open file\n");
        return;
    }

    printf("File opened successfully\n");

    char header[4];

    fread(header,1,3,fp);

    header[3]='\0';

    printf("=====================================\n");
    printf("          MP3 TAG READER\n");
    printf("=====================================\n");

    char version[2];  //to fetch version

    fread(version,1,2,fp);   // to read the version 

    printf("Version : 2.%d.%d\n\n", version[0], version[1]);

    fseek(fp,5,SEEK_CUR);    //toskip 5 bytes from cureetn position
    read_frame(fp);
    fclose(fp);
}


void read_frame(FILE *fp)
{
    char frame_id[5];    //array for frame id size 5 because last is \0 treat as string

    while (1)
    {
        // Read Frame ID
        if (fread(frame_id, 1, 4, fp) != 4)
        {
            break;
        }

        frame_id[4] = '\0';

        //printf("Frame ID : %s\n", frame_id);

        // Read Frame Size
        unsigned char size[4];    //unsigned because to avoid negative value

        if (fread(size, 1, 4, fp) != 4)
        {
            break;
        }

        int frame_size;

        // Convert 4 bytes into integer (Big Endian format)
        frame_size = (size[0] << 24) |(size[1] << 16) |(size[2] << 8) |size[3];

        //printf("Frame Size : %d\n", frame_size);

        fseek(fp, 2, SEEK_CUR);     // Skip 2 bytes of flags

        if(strcmp(frame_id, "TIT2") == 0)           //comparing frame id it is TIT2 or not
        {
            display_tag(fp, frame_size, "Title");
        }
        else if(strcmp(frame_id, "TPE1") == 0)      //comparing frame id it is TPE1 or not
        {
            display_tag(fp, frame_size, "Artist");
        }
        else if(strcmp(frame_id, "TALB") == 0)      //comparing frame id it is TALB or not
        {
            display_tag(fp, frame_size, "Album");
        }
        else if(strcmp(frame_id, "TCOM") == 0)     //comparing frame id it is TCOM  or not
        {
            display_tag(fp, frame_size, "Composer");
        }
        else if(strcmp(frame_id, "TYER") == 0)      //comparing frame id it is TYER or not
        {
            display_tag(fp, frame_size, "Year");
        }
        else if(strcmp(frame_id, "TCON") == 0)      //comparing frame id it is TCON or not
        {
            display_tag(fp, frame_size, "Genre");
        }
        else if(strcmp(frame_id, "COMM") == 0)
        {
            display_tag(fp, frame_size, "Comment");
        }
        else
        {
        fseek(fp, frame_size, SEEK_CUR);    // Skip unwanted frame data
        }

        //printf("-------------------------\n");    // work like seperator
    }
}


void display_tag(FILE *fp, int frame_size, char *tag_name)
{
    char encoding;      // Stores text encoding (00 or 01)

    fread(&encoding, 1, 1, fp);  // Read 1 byte encoding

    char tag_value[100];      //to store the artist name, title of song likewise

    fread(tag_value, 1, frame_size - 1, fp);  // Read actual tag value  
    tag_value[frame_size - 1] = '\0';    // string end null at last position

    printf("%s : %s\n", tag_name, tag_value);
}