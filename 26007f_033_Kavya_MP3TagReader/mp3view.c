#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mp3view(char *filename)
{
    FILE *fp;   

    char frame_id[5] = {0}; // to store the frame id of the ID3 tag
    unsigned char size_bytes[4]; // to store the size of the frame
    unsigned char frame_flags[2]; // to store the flags of the frame

    unsigned int size; // to store the size of the ID3 tag

    char title[256]; // to store the title of the song
    char artist[256]; // to store the artist of the song
    char album[256]; // to store the album of the song
    char year[256]; // to store the year of the song
    char content[256]; // to store the content of the song
    char comment[256]; // to store the comment of the song

    int title_found = 0;
    int artist_found = 0;
    int album_found = 0;
    int year_found = 0;
    int content_found = 0;
    int comment_found = 0;

    fp = fopen(filename, "rb"); // open the file in read mode
    if(fp == NULL) // check file is present or not
    {
        printf("ERROR: File Not Found\n");
        return 0;
    }

    fseek(fp, 10, SEEK_SET); // move the file pointer to the 10th byte of the file(skip header)

    printf("SELECTED VIEW DETAILS\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("------------------------MP3 TAG INFORMATION-----------------------------------\n"); 
    printf("------------------------------------------------------------------------------------\n");
    
    while(1)
    {
        if(fread(frame_id, 1, 4, fp) != 4) // read 4 bytes of frame id
        {
            break;
        }
        frame_id[4] = '\0'; // add null character at the end of the frame id
        if(frame_id[0] == '\0') // frame id starts with 0 end the frames
        {
            break;
        }
        if(fread(size_bytes, 1, 4, fp) != 4)
        {
            break;
        }
        if(fread(frame_flags, 1, 2, fp) != 2) // read frame flags     
        {
            break;
        }
        size = ((unsigned int)size_bytes[0] << 24) | ((unsigned int)size_bytes[1] << 16) | ((unsigned int)size_bytes[2] << 8) | ((unsigned int)size_bytes[3]); // convert size_bytes to size
        if(size > 255) // prevent buffer overflow for large size values
        {
            fseek(fp, size, SEEK_CUR);
            continue;
        }
        if(strcmp(frame_id, "TIT2") == 0) // check frame id is TIT2
        {
            unsigned char encoding; // to store the encoding of the title
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(title, 1, size - 1, fp); // read the title of the song
                title[size - 1] = '\0'; // add null character at the end of the title
                printf("TITLE   : %s\n", title); // print the title of the song
                title_found = 1; // set title_found to 1
            }
        }

        else if(strcmp(frame_id, "TPE1") == 0) // check frame id is TPE1
        {
            unsigned char encoding; // to store the encoding of the artist
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(artist, 1, size - 1, fp); // read the artist of the song
                artist[size - 1] = '\0'; // add null character at the end of the artist
                printf("ARTIST  : %s\n", artist); // print the artist of the song
                artist_found = 1; // set artist_found to 1
            }
        }

        else if(strcmp(frame_id, "TALB") == 0) // check frame id is TALB
        {
            unsigned char encoding; // to store the encoding of the album
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(album, 1, size - 1, fp); // read the album of the song
                album[size - 1] = '\0'; // add null character at the end of the album
                printf("ALBUM   : %s\n", album); // print the album of the song
                album_found = 1; // set album_found to 1
            }
        }

        else if(strcmp(frame_id, "TYER") == 0) // check frame id is TYER
        {
            unsigned char encoding; // to store the encoding of the year
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(year, 1, size - 1, fp); // read the year of the song
                year[size - 1] = '\0'; // add null character at the end of the year
                printf("YEAR    : %s\n", year); // print the year of the song
                year_found = 1; // set year_found to 1
            }
        }

        else if(strcmp(frame_id, "TCON") == 0) // check frame id is TCON
        {
            unsigned char encoding; // to store the encoding of the content
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(content, 1, size - 1, fp); // read the content of the song
                content[size - 1] = '\0'; // add null character at the end of the content
                printf("CONTENT : %s\n", content); // print the content of the song
                content_found = 1; // set content_found to 1
            }
        }

        else if(strcmp(frame_id, "COMM") == 0) // check frame id is TCOM
        {
            unsigned char encoding; // to store the encoding of the comment
            if(size > 0)
            {
                fread(&encoding, 1, 1, fp); // read the encoding byte
                fread(comment, 1, size - 1, fp); // read the comment of the song
                comment[size - 1] = '\0'; // add null character at the end of the comment
                printf("COMMENT : %s\n", comment); // print the comment of the song
                comment_found = 1; // set comment_found to 1
            }
        }
        else
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
        }
    }
    if(!title_found)
        printf("TITLE   : Not Found\n");

    if(!artist_found)
        printf("ARTIST  : Not Found\n");

    if(!album_found)
        printf("ALBUM   : Not Found\n");

    if(!year_found)
        printf("YEAR    : Not Found\n");

    if(!content_found)
        printf("CONTENT : Not Found\n");

    if(!comment_found)
        printf("COMMENT : Not Found\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("Displayed Successfully\n");
    fclose(fp); // close the file
    return 1;   
}