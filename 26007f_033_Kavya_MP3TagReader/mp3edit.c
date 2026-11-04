#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mp3edit(char *filename, char *option, char *new_value)
{
    FILE *fp;

    unsigned char size_bytes[4]; // to store the size of the frame
    unsigned char frame_flags[2]; // to store the flags of the ID3 tag
    unsigned int size; // to store the size of the ID3 tag

    char frame_id[5] = {0}; // to store the frame id of the ID3 tag

    char title[256];
    char artist[256];
    char album[256];
    char year[256];
    char content[256];
    char comment[256];

    fp = fopen(filename, "rb+"); // open the file in read and write mode
    if(fp == NULL) // check file is present or not
    {
        return 0;
    }

    fseek(fp, 10, SEEK_SET); // move the file pointer to the 10th byte of the file(skip header)

    printf("Selected Edit Details\n");
    printf("Selected Edit Option: %s\n", option);
    printf("New Value: %s\n", new_value);

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
    if(fread(size_bytes, 1, 4, fp) != 4) // read 4 bytes of size
    {
        break;
    }
    if(fread(frame_flags, 1, 2, fp) != 2) // read 2 bytes of flags
    {
        break;
    }
    size = ((unsigned int)size_bytes[0] << 24) | ((unsigned int)size_bytes[1] << 16) | ((unsigned int)size_bytes[2] << 8) | ((unsigned int)size_bytes[3]); // convert size_bytes to size

    if(strcmp(option, "-t") == 0 && strcmp(frame_id, "TIT2") == 0) // check cmd line is -t and frame id is TIT2
    {
        unsigned char encoding; // to store the encoding of the title
        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(title, 1, size - 1, fp); // read the title of the song
        title[size - 1] = '\0'; // add null character at the end of the title
        if(strlen(new_value) > size - 1)
        {
            printf("ERROR: New title is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(size - 1), SEEK_CUR); // move the file pointer to the start of the title
        memset(title, 0, size - 1); // clear old title space
        memcpy(title, new_value, strlen(new_value)); // copy new title to title variable
        fwrite(title, 1, size - 1, fp); // write the new title to the file
        printf("Title : %s\n", new_value);
        printf("Title is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }

    else if(strcmp(option, "-a") == 0 && strcmp(frame_id, "TPE1") == 0) // check the cmd line is artist and frame id is TPE1
    {
        unsigned char encoding; // to store the encoding of the artist
        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(artist, 1, size - 1, fp); // read the artist of the song
        artist[size - 1] = '\0'; // add null character at the end of the artist
        if(strlen(new_value) > size - 1)
        {
            printf("ERROR: New artist is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(size - 1), SEEK_CUR); // move the file pointer to the start of the artist
        memset(artist, 0, size - 1); // clear old artist space
        memcpy(artist, new_value, strlen(new_value)); // copy new artist to artist variable
        fwrite(artist, 1, size - 1, fp); // write the new artist to the file
        printf("Artist : %s\n", new_value);
        printf("Artist is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }


    else if(strcmp(option, "-A") == 0 && strcmp(frame_id, "TALB") == 0) // check cmd line is album and frame id is TALB
    {
        unsigned char encoding; // to store the encoding of the album
        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(album, 1, size - 1, fp); // read the album of the song
        album[size - 1] = '\0'; // add null character at the end of the album
        if(strlen(new_value) > size - 1)
        {
            printf("ERROR: New album is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(size - 1), SEEK_CUR); // move the file pointer to
        memset(album, 0, size - 1); // clear old album space
        memcpy(album, new_value, strlen(new_value)); // copy new album to album variable
        fwrite(album, 1, size - 1, fp); // write the new album to the file
        printf("Album : %s\n", new_value);
        printf("Album is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }

    else if(strcmp(option, "-y") == 0 && strcmp(frame_id, "TYER") == 0) // check cmd line is year and frame id is TYER
    {
        unsigned char encoding; // to store the encoding of the year
        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(year, 1, size - 1, fp); // read the year of the song
        year[size - 1] = '\0'; // add null character at the end of the year
        if(strlen(new_value) > size - 1)
        {
            printf("ERROR: New year is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(size - 1), SEEK_CUR); // move the file pointer to the start of the year
        memset(year, 0, size - 1); // clear old year space
        memcpy(year, new_value, strlen(new_value)); // copy new year to year variable
        fwrite(year, 1, size - 1, fp);// write the new year to the file
        printf("Year : %s\n", new_value);
        printf("Year is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }

    else if(strcmp(option, "-m") == 0 && strcmp(frame_id, "TCON") == 0) // check cmd line is content and frame id is TCON
    {
        unsigned char encoding; // to store the encoding of the content
        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(content, 1, size - 1, fp); // read the content of the song
        content[size - 1] = '\0'; // add null character at the end of the content
        if(strlen(new_value) > size - 1)
        {
            printf("ERROR: New content is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(size - 1), SEEK_CUR); // move the file pointer to the start of the content
        memset(content, 0, size - 1); // clear old content space
        memcpy(content, new_value, strlen(new_value)); // copy new content to content variable
        fwrite(content, 1, size - 1, fp); // write the new content to the file
        printf("Content : %s\n", new_value);
        printf("Content is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }

    else if(strcmp(option, "-c") == 0 && strcmp(frame_id, "COMM") == 0) // check cmd line is comment and frame id is TCOM
    {
        unsigned char encoding; // to store the encoding of the comment
        unsigned char language[3];
        unsigned char description;

        if(size == 0)
            break;
        if(size > 255)
        {
            fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
            continue;
        }
        fread(&encoding, 1, 1, fp); // read the encoding byte
        fread(language, 1, 3, fp);
        do
        {
            fread(&description, 1, 1, fp);
        } while(description != '\0');
        unsigned int comment_size = size - 5;
        fread(comment, 1, comment_size, fp); // read the comment of the song
        comment[comment_size] = '\0'; // add null character at the end of the comment

        if(strlen(new_value) > comment_size)
        {
            printf("ERROR: New comment is too long\n");
            fclose(fp);
            return 0;
        }
        fseek(fp, -(long)comment_size, SEEK_CUR); // move the file pointer to the start of the comment
        memset(comment, 0, comment_size); // clear old comment space
        memcpy(comment, new_value, strlen(new_value)); // copy new comment to comment variable
        fwrite(comment, 1, comment_size, fp); // write the new comment to the file
        printf("Comment : %s\n", new_value);
        printf("Comment is Edited Successfully\n");
        fclose(fp); // close the file
        return 1;
    }
    else
    {
        fseek(fp, size, SEEK_CUR); // move the file pointer to the next frame
    }
}
    fclose(fp); // close the file
    return 1;
}