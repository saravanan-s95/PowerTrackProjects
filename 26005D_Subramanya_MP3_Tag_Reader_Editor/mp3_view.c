/*
 * ID3v2.3 notes:
 * - Tag size is a synchsafe integer.
 * - Frame size is a normal 32-bit big-endian integer.
 * - Text frame data starts with an encoding byte.
 * - COMM has encoding + language + description before comment text.
 *
 *
 * VIEW ALGORITHM:
 *
 * 1. Open the MP3 file in read mode.
 *
 * 2. Read and validate the ID3v2.3 header.
 *
 * 3. Decode the tag size from the header.
 *
 * 4. Process each frame until the tag ends:
 *    → Read Frame ID, size, flags and data.
 *    → Check the Frame ID.
 *    → Display the required tag information.
 *
 * 5. If padding is found:
 *    → Stop reading frames.
 *
 * 6. Close the MP3 file.
 * 
 */

#include "mp3_header.h"

void view(MP3_t *mp3)
{
    /* Open file and validate */
    FILE *fp = fopen(mp3->mp3_filename, "rb");
    if(fp == NULL)
    {
        printf("\nERROR: Could not open MP3 file!\n");
        return;
    }

    printf("\n");
    printf("========================================\n");
    printf("           MP3 TAG INFORMATION\n");
    printf("========================================\n");
    printf("File : %s\n", mp3->mp3_filename);
    printf("----------------------------------------\n");

    print_tag_data(fp);

    printf("========================================\n\n");

    fclose(fp);
}



/* Sample ID3v2.3 header and frame layout:
 *
 * Header: (10 bytes in size)
 * 3 bytes -> File Identifier "ID3"
 * 2 bytes -> Version v2.3 (03)
 * 1 byte  -> Flags
 * 4 bytes -> Tag size (synchsafe)
 *
 * Frame:
 * 4 bytes -> Frame ID
 * 4 bytes -> Frame size (big endian) store in varibale fsize
 * 2 bytes -> Flags
 * N bytes -> Frame data read using fsize 
 */

void print_tag_data(FILE *fp)
{
    unsigned char header[HEADER_SIZE];

    /* Read header 10 bytes */
    if(fread(header, 1, HEADER_SIZE, fp) != HEADER_SIZE)
    {
        printf("ERROR: Unable to read the complete ID3 header!\n");
        return;
    }

    /* First 3 bytes must be "ID3" and version must be 2.3 */
    if(header[0] != 'I' || header[1] != 'D' || header[2] != '3' ||
       header[3] != 3)
    {
        printf("ERROR: The given file does not contain a valid ID3v2.3 tag!\n");
        return;
    }

    /* Reconstruct tag_size from its 4 synchsafe bytes */
    int tag_size = (header[6] << 21) |
                   (header[7] << 14) |
                   (header[8] << 7)  |
                    header[9];

    /* Number of tag-body bytes processed so far */
    int bytes_read = 0;

    while(bytes_read < tag_size)
    {
        /* Read one frame */
        unsigned char frame_id[ID_SIZE + 1];

        /* Read the 4-byte frame ID */
        if(fread(frame_id, 1, ID_SIZE, fp) != ID_SIZE)
        {
            printf("ERROR: Unable to read the complete frame ID!\n");
            return;
        }

        frame_id[ID_SIZE] = '\0';

        /*
         * 00 means there is no valid frame here.
         * Remaining tag space is padding, so stop reading.
         */
        if(frame_id[0] == 0)
            break;


        /* Read the normal 32-bit big-endian frame size */
        unsigned char frame_size[FRAME_SIZE];

        if(fread(frame_size, 1, FRAME_SIZE, fp) != FRAME_SIZE)
        {
            printf("ERROR: Unable to read the complete frame size!\n");
            return;
        }

        /* Reconstruct from big-endian frame size */
        int fsize = (frame_size[0] << 24) |
                    (frame_size[1] << 16) |
                    (frame_size[2] << 8)  |
                     frame_size[3];


        /* Check frame size before creating data buffer */
        if(bytes_read + ID_SIZE + FRAME_SIZE + FLAG_OFFSET + fsize > tag_size)
        {
            printf("ERROR: Invalid frame size!\n");
            return;
        }


        /* Move it forward by 2 bytes to skip the frame flags */
        if(fseek(fp, FLAG_OFFSET, SEEK_CUR) != 0)
        {
            printf("ERROR: Unable to skip frame flags!\n");
            return;
        }


        /* Read the actual frame data as a string */
        char data[fsize + 1];

        if(fread(data, 1, fsize, fp) != (size_t)fsize)
        {
            printf("ERROR: Unable to read the complete frame content!\n");
            return;
        }

        data[fsize] = '\0';


        /* Text frames: byte 0 = encoding, actual text starts at byte 1 */
        if(strcmp((char *)frame_id, "TIT2") == 0)
            printf("Title   : %s\n", data + 1);

        else if(strcmp((char *)frame_id, "TPE1") == 0)
            printf("Artist  : %s\n", data + 1);

        else if(strcmp((char *)frame_id, "TALB") == 0)
            printf("Album   : %s\n", data + 1);

        else if(strcmp((char *)frame_id, "TYER") == 0)
            printf("Year    : %s\n", data + 1);

        else if(strcmp((char *)frame_id, "TCON") == 0)
            printf("Music   : %s\n", data + 1);

        /* COMM: encoding + language + description + comment */
        else if(strcmp((char *)frame_id, "COMM") == 0)
        {
            /* Skip encoding + 3-byte language, then find description terminator */
            char *comment = (char *)data + 4;

            while(*comment != '\0')
                comment++;

            comment++;  /* Move past description's null terminator */

            printf("Comment : %s\n", comment);
        }

        /* Update total tag-body bytes processed */
        bytes_read += ID_SIZE + FRAME_SIZE + FLAG_OFFSET + fsize;
    }
}