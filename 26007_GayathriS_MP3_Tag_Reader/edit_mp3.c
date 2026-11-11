#include<stdio.h>     
#include<stdlib.h>    
#include<string.h>     
#include"main.h"       

// Function to map user option (-t, -A, -a, etc.) to ID3 frame name
const char *option_frame(const char *opt)
{
    if (!strcmp(opt, "-t"))          // If option is -t
        return "TIT2";              // Return frame ID for Title
    if (!strcmp(opt, "-A"))          // If option is -A
        return "TALB";              // Return frame ID for Album
    if (!strcmp(opt, "-a"))          // If option is -a
        return "TPE1";              // Return frame ID for Artist
    if (!strcmp(opt, "-y"))          // If option is -y
        return "TYER";              // Return frame ID for Year
    if (!strcmp(opt, "-C"))          // If option is -C
        return "TCOM";              // Return frame ID for Composer
    if (!strcmp(opt, "-c"))          // If option is -c
        return "TCON";              // Return frame ID for Genre
    return NULL;                     // Return NULL if option not recognized
}

// Function to edit MP3 tags
void edit_mp3_tags(const char *tag_name, const char *new_value, const char *filename)
{
    // Open the original MP3 file in read+write mode
    FILE *fp = fopen(filename, "r+");   
    if (fp == NULL)                          // If file not found
    {
        printf("ERROR : Unable to open file %s\n", filename);
        return;                              
    }

    // Create a temporary MP3 file for modified data
    FILE *temp = fopen("temp.mp3", "w+");    
    if (temp == NULL)                        // If creation fails
    {
        printf("ERROR : Unable to create temp file.\n");
        fclose(fp);                          // Close original file
        return;
    }

    // Read the 10-byte MP3 header
    char header[10];                         
    if(fread(header, 1, 10, fp) != 10)       // If header not read properly
    {
        printf("ERROR : Unable to read MP3 header.\n");
        fclose(fp);
        fclose(temp);
        return;
    }

    // Write the header to temp file
    if(fwrite(header, 1, 10, temp) != 10)    
    {
        printf("ERROR : Unable to write MP3 header.\n");
        fclose(temp);
        fclose(fp);
        return;
    }

    int size_diff = 0;   // Keep track of difference in tag sizes

    // Loop through each frame in the MP3
    while (1)
    {
        char tag[5] = {0};                  // To store frame ID (4 chars + null terminator)
        unsigned char size_bytes[4];        // To store frame size in 4 bytes
        unsigned char flags[2];             // To store frame flags

        if (fread(tag, 1, 4, fp) != 4)      // Read frame ID (4 bytes)
            break;                          // Stop if cannot read

        if(tag[0] == 0)                     // If reached padding or end of frames
        {
            fwrite(tag, 1, 4, temp);        // Write same data to temp
            break;                          // Exit loop
        }

        if (fread(size_bytes, 1, 4, fp) != 4)  // Read frame size (4 bytes)
            break;

        // Convert size from 4 bytes to integer (big endian)
        int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | 
                   (size_bytes[2] << 8) | size_bytes[3];

        if (fread(flags, 1, 2, fp) != 2)    // Read frame flags (2 bytes)
            break;

        // Allocate memory for frame data
        char *data = (char *)malloc(size);  
        if (!data)                          // If memory allocation fails
        {
            printf("Memory allocation failed.\n");
            fclose(fp);
            fclose(temp);
            return;
        }

        fread(data, 1, size, fp);           // Read frame data from file

        if (strcmp(tag, tag_name) == 0)     // If this frame matches the one we want to edit
        {
            int new_size = strlen(new_value) + 1;  // New size (text + encoding byte)
            size_diff += (new_size - size);        // Track size difference

            // Convert new size to 4 bytes (big endian)
            unsigned char new_size_bytes[4] = {
                (new_size >> 24) & 0xFF, 
                (new_size >> 16) & 0xFF, 
                (new_size >> 8) & 0xFF, 
                new_size & 0xFF
            };

            fwrite(tag, 1, 4, temp);               // Write frame ID
            fwrite(new_size_bytes, 1, 4, temp);    // Write new size
            fwrite(flags, 1, 2, temp);             // Write flags
            fputc(0x00, temp);                     // Write encoding byte (0 = ISO-8859-1)
            fwrite(new_value, 1, strlen(new_value), temp);  // Write new tag value

            printf("Edited tag %s successfully.\n", tag_name);
        }
        else   // If frame is not being edited, copy as is
        {
            fwrite(tag, 1, 4, temp);               // Write frame ID
            fwrite(size_bytes, 1, 4, temp);        // Write size
            fwrite(flags, 1, 2, temp);             // Write flags
            fwrite(data, 1, size, temp);           // Write old data
        }

        free(data);  // Free allocated memory
    }

    // Buffer for copying remaining audio data
    char buff[100000];               
    size_t n;

    // Copy rest of the MP3 file (audio frames)
    while((n = fread(buff, 1, sizeof(buff), fp)) > 0)
    {
        if(fwrite(buff, 1, n, temp) != n)   // Write to temp file
        {
            printf("Error : Failed to write remaining data.\n");
            break;
        }
    }

    fclose(fp);      // Close original file
    fclose(temp);    // Close temp file

    // Replace original file with updated temp file
    if (remove(filename) != 0 || rename("temp.mp3", filename) != 0)
    {
        printf("ERROR : Failed to update file.\n");
    }
    else
    {
        printf("<---Edited Information Successfully--->\n");
    }
}
