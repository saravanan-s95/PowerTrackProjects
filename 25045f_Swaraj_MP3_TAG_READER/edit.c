#include <stdio.h>
#include <string.h>
#include "edit.h"

void edit_tag(char *option, char *new_data, char *filename)
{
    printf("Option   : %s\n", option);
    printf("New Data : %s\n", new_data);
    printf("File     : %s\n", filename);

    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL)
    {
        printf("Error : Unable to open %s\n", filename);
        return;
    }

    FILE *ftemp = fopen("temp.mp3", "wb");    //taking temporary because foe edit we are going to copy

    if (ftemp == NULL)
    {
        printf("Error : Unable to create temp file\n");

        fclose(fptr);
        return;
    }
   
    char buffer[10];   //array to store the 10 byte sof header

    // Read first 10 bytes (ID3 Header) from original file
    fread(buffer, 1, 10, fptr);

    // Write same 10 bytes into temp file   
    fwrite(buffer, 1, 10, ftemp);

    char frame_id[5];   // array to store fram id 

    while (1)
    {
    // Read Frame ID
    if (fread(frame_id, 1, 4, fptr) != 4)
    {
        break;
    }

    frame_id[4] = '\0';

    // Read Frame Size
    unsigned char size[4];
    unsigned char new_size[4];    // to read the frame size after edit 

    fread(size, 1, 4, fptr);

    int frame_size;

    frame_size = (size[0] << 24) |(size[1] << 16) |(size[2] << 8) |size[3];

    // Stop if frame size is invalid
    if (frame_size <= 0 || frame_size > 100000)
    {
        printf("End of valid frames\n");
        break;
    }

    // Read Flags
    char flags[2];

    fread(flags, 1, 2, fptr);

    // Read Frame Data
    char data[frame_size];

    fread(data, 1, frame_size, fptr);

    // Check whether Title frame
    if (strcmp(frame_id, "TIT2") == 0 && strcmp(option, "-t") == 0)
    {
        int new_frame_size;

        new_frame_size = strlen(new_data) + 1;   //strlen to count the lenght and +1 because ID3 has 1 encoding byte 
        
        //to convert from integer to bytes
        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;    
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        printf("Old Frame Size : %d\n", frame_size);
        printf("New Frame Size : %d\n", new_frame_size);

        // Write Frame ID
        fwrite(frame_id, 1, 4, ftemp);

        // Write New Frame Size
        fwrite(new_size, 1, 4, ftemp);

        // Write Flags
        fwrite(flags, 1, 2, ftemp);

        // Write Encoding Byte
        fputc(0, ftemp);

        // Write New Title
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Title Updated\n");
    }

    //check for artist name
    else if (strcmp(frame_id, "TPE1") == 0 && strcmp(option, "-a") == 0)
    {
        int new_frame_size = strlen(new_data) + 1;

        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        fwrite(frame_id, 1, 4, ftemp);
        fwrite(new_size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);

        fputc(0, ftemp);
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Artist Updated\n");
    }

    //check for album name
    else if (strcmp(frame_id, "TALB") == 0 && strcmp(option, "-A") == 0)
    {
        int new_frame_size = strlen(new_data) + 1;

        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        fwrite(frame_id, 1, 4, ftemp);
        fwrite(new_size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);

        fputc(0, ftemp);
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Album Updated\n");
    }

    //check year 
    else if (strcmp(frame_id, "TYER") == 0 && strcmp(option, "-y") == 0)
    {
        int new_frame_size = strlen(new_data) + 1;

        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        fwrite(frame_id, 1, 4, ftemp);
        fwrite(new_size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);

        fputc(0, ftemp);
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Year Updated\n");
    }

    //check genre
    else if (strcmp(frame_id, "TCON") == 0 && strcmp(option, "-g") == 0)
    {
        int new_frame_size = strlen(new_data) + 1;

        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        fwrite(frame_id, 1, 4, ftemp);
        fwrite(new_size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);

        fputc(0, ftemp);
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Genre Updated\n");
    }

    //chcek for comments
    else if (strcmp(frame_id, "COMM") == 0 && strcmp(option, "-c") == 0)
    {
        int new_frame_size = strlen(new_data) + 1;

        new_size[0] = (new_frame_size >> 24) & 0xFF;
        new_size[1] = (new_frame_size >> 16) & 0xFF;
        new_size[2] = (new_frame_size >> 8) & 0xFF;
        new_size[3] = new_frame_size & 0xFF;

        fwrite(frame_id, 1, 4, ftemp);
        fwrite(new_size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);

        fputc(0, ftemp);
        fwrite(new_data, 1, strlen(new_data), ftemp);

        printf("Comment Updated\n");
    }
    else
    {
        fwrite(frame_id, 1, 4, ftemp);
        fwrite(size, 1, 4, ftemp);
        fwrite(flags, 1, 2, ftemp);
        fwrite(data, 1, frame_size, ftemp);
    }
    }

    fclose(fptr);   // fptr close
    fclose(ftemp);  // temp close

    printf("Files closed successfully\n");

    remove(filename);           // remove original file
    rename("temp.mp3", filename);    // rename the temp file to original file name

    printf("Tag Edited Successfully\n");

}