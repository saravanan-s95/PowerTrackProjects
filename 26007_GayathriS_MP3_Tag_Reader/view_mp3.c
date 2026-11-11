#include <stdio.h>      
#include <stdlib.h>    
#include <string.h>    
#include "main.h"      

// Function to display 6 MP3 tags 
void view_mp3_tags(const char* filename)
{
    FILE *fp = fopen(filename, "r+");                // Open MP3 file in read+write mode
    if (fp == NULL)                                  // Check if file opened successfully
    {
        printf("ERROR : In opening file.\n");        // Print error if failed
        return;                                      
    }

    char header[4] = {0};                            // Buffer to hold the header
    if(fread(header, 1, 3, fp) != 3)                 // Read first 3 bytes ("ID3")
    {
        printf("ERROR : In reading the header.\n");  // Error if read failed
        fclose(fp);                                  
        return;                                     
    }
    header[3] = '\0';                                // Null terminate the string

    if (strcmp(header, "ID3") != 0)                  // Check if file starts with "ID3"
    {
        printf("Error: Not a valid MP3 file with ID3 tag.\n");  
        fclose(fp);                                  // Close file if not valid
        return;                                      
    }

    // Skip the rest of the ID3 header 
    fseek(fp, 7, SEEK_CUR);

    // Tag identifiers (frames) in ID3 format
    const char *tags[6]  = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "TCOM"};
    // Human-readable names for printing
    const char *names[6] = {"TITLE", "ARTIST", "ALBUM", "YEAR", "CONTENT", "COMPOSER"};

    printf("\nMP3 Tag Reader Data");
    printf("\n------------------------------------------------------\n");

    // Loop through 6 tags to read and display them
    for (int i = 0; i < 6; i++)
    {
        char tag[5] = {0};                           // Buffer for frame ID (4 chars + null)
        if(fread(tag, 1, 4, fp) != 4)                // Read tag name (e.g., TIT2)
        {
            break;                                   // Stop if cannot read
        }

        unsigned char size_bytes[4];                
        if(fread(size_bytes, 1, 4, fp) != 4)         // Read frame size
        {
            break;                                   // Stop if cannot read
        } 

        // Convert 4 size bytes into an integer
        int size = (size_bytes[0] << 24) | 
                   (size_bytes[1] << 16) | 
                   (size_bytes[2] << 8)  | 
                    size_bytes[3];

        fseek(fp, 2, SEEK_CUR);                      // Skip frame flags (2 bytes)

        if(size <= 0)                                // If size is invalid, skip
        {
            continue;
        }

        char *data = (char *)malloc(size+1);         // Allocate memory for frame data
        if (data == NULL)                            // Check if malloc failed
        {
            printf("Memory allocation failed");      
            fclose(fp);                              // Close file
            return;                                  
        }

        if(fread(data, 1, size, fp) != size)         // Read frame data
        {
            free(data);                              // Free memory if read fails
            return;                                  
        }

        data[size] = '\0';                           // Null terminate the string

        // Print tag name with its value (skip first byte because it's encoding info)
        printf("%-8s : %s\n", names[i], data+1);

        free(data);                                  // Free allocated memory
    }

    printf("------------------------------------------------------\n");

    fclose(fp);                                      // Close the file
}
