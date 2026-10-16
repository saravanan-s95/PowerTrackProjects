/*
 * EDIT ALGORITHM:
 *
 * 1. Open the original MP3 and create a temporary file.
 *
 * 2. Read and validate the ID3v2.3 header.
 *    → Copy the header to the temporary file.
 *
 * 3. Read the tag size and process each frame:
 *    → Read Frame ID, size, flags and data.
 *    → Check if the Frame ID matches the requested tag.
 *
 * 4. If a match is found:
 *    → Keep the required prefix.
 *    → Replace the old text with the new value.
 *    → Write the updated frame to the temporary file.
 *    → Calculate the new tag size.
 *
 * 5. If no match:
 *    → Copy the original frame unchanged.
 *
 * 6. Copy the remaining file data to the temporary file.
 *
 * 7. If a tag was edited:
 *    → Convert the new tag size to synchsafe format.
 *    → Update the size in the temporary header.
 *    → Replace the original MP3 with the temporary file.
 *
 * 8. If no match was found:
 *    → Delete the temporary file.
 */

#include "mp3_header.h"

void edit(MP3_t *mp3)
{
    /* Open original MP3 file in rb mode */
    FILE *org_fp = fopen(mp3->mp3_filename, "rb");
    if(org_fp == NULL)
    {
        printf("\nERROR: Could not open MP3 file!\n");
        return;
    }

    /* Create temporary file */
    FILE *dup_fp = fopen("temp.mp3", "wb");
    if(dup_fp == NULL)
    {
        printf("\nERROR: Could not create temporary file!\n");
        fclose(org_fp);
        return;
    }


    /* Read and validate ID3 header */
    unsigned char header[HEADER_SIZE];

    if(fread(header, 1, HEADER_SIZE, org_fp) != HEADER_SIZE)
    {
        printf("\nERROR: Unable to read the complete ID3 header!\n");
        fclose(org_fp);
        fclose(dup_fp);
        remove("temp.mp3");
        return;
    }


    /* First 3 bytes must be "ID3" and version must be 2.3 */
    if(header[0] != 'I' || header[1] != 'D' || header[2] != '3' ||
       header[3] != 3)
    {
        printf("\nERROR: The given file does not contain a valid ID3v2.3 tag!\n");

        fclose(org_fp);
        fclose(dup_fp);
        remove("temp.mp3");

        return;
    }


    /* Copy the original header to the temporary file */
    fwrite(header, HEADER_SIZE, 1, dup_fp);


    /* Reconstruct tag_size */
    int tag_size = (header[6] << 21) |
                   (header[7] << 14) |
                   (header[8] << 7)  |
                    header[9];
                    

    int bytes_read = 0;
    int matchFound = 0;
    int new_tag_size = tag_size;


    while(bytes_read < tag_size)
    {
        /* Read frame ID */
        unsigned char frame_id[ID_SIZE + 1];

        if(fread(frame_id, 1, ID_SIZE, org_fp) != ID_SIZE)
        {
            printf("\nERROR: Unable to read the complete frame ID!\n");
            fclose(org_fp);
            fclose(dup_fp);
            remove("temp.mp3");
            return;
        }

        frame_id[ID_SIZE] = '\0';


        /* 00 means padding; no more frames */
        if(frame_id[0] == 0)
        {
            /* Copy the 4 padding bytes already read */
            fwrite(frame_id, 1, ID_SIZE, dup_fp);
            break;
        }


        /* Read frame size */
        unsigned char frame_size[FRAME_SIZE];

        if(fread(frame_size, 1, FRAME_SIZE, org_fp) != FRAME_SIZE)
        {
            printf("\nERROR: Unable to read the complete frame size!\n");
            fclose(org_fp);
            fclose(dup_fp);
            remove("temp.mp3");
            return;
        }


        /* Convert big-endian bytes into integer */
        int fsize = (frame_size[0] << 24) |
                    (frame_size[1] << 16) |
                    (frame_size[2] << 8)  |
                     frame_size[3];


        /* Check frame size before creating data buffer */
        if(bytes_read + ID_SIZE + FRAME_SIZE + FLAG_OFFSET + fsize > tag_size)
        {
            printf("\nERROR: Invalid frame size!\n");
            fclose(org_fp);
            fclose(dup_fp);
            remove("temp.mp3");
            return;
        }


        /* Read flags because unchanged frames need to be copied */
        unsigned char flags[FLAG_OFFSET];

        if(fread(flags, 1, FLAG_OFFSET, org_fp) != FLAG_OFFSET)
        {
            printf("\nERROR: Unable to read the complete frame flags!\n");
            fclose(org_fp);
            fclose(dup_fp);
            remove("temp.mp3");
            return;
        }


        /* Read actual frame data */
        unsigned char data[fsize];

        if(fread(data, 1, fsize, org_fp) != (size_t)fsize)
        {
            printf("\nERROR: Unable to read the complete frame content!\n");
            fclose(org_fp);
            fclose(dup_fp);
            remove("temp.mp3");
            return;
        }


        /* Check whether this is the frame to edit */
        if(strcmp(mp3->tag_id, (char *)frame_id) == 0)
        {
            matchFound = 1;


            /* Keep the frame-specific prefix */
            int prefix_length;

            if(strcmp((char *)frame_id, "COMM") == 0)
            {
                /* Skip encoding + language */
                prefix_length = 4;

                /* Find description's null terminator */
                while(prefix_length < fsize && data[prefix_length] != '\0')
                    prefix_length++;

                /* Description terminator must exist */
                if(prefix_length >= fsize)
                {
                    printf("\nERROR: Invalid COMM frame!\n");
                    fclose(org_fp);
                    fclose(dup_fp);
                    remove("temp.mp3");
                    return;
                }

                prefix_length++;  /* Include description's null terminator */
            }
            else
            {
                prefix_length = 1;
            }


            /* Compute new frame size */
            int new_size = prefix_length + strlen(mp3->new_value);


            /* Copy prefix and new text into data buffer */
            unsigned char data_buffer[new_size];

            for(int i = 0, j = 0; i < new_size; i++)
            {
                if(i < prefix_length)
                    data_buffer[i] = data[i];
                else
                    data_buffer[i] = mp3->new_value[j++];
            }


            /* Convert new frame size to 4-byte big-endian */
            unsigned char new_size_bytes[FRAME_SIZE];

            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8)  & 0xFF;
            new_size_bytes[3] = new_size & 0xFF;


            /* Write the replacement frame */
            fwrite(frame_id, 1, ID_SIZE, dup_fp);
            fwrite(new_size_bytes, 1, FRAME_SIZE, dup_fp);
            fwrite(flags, 1, FLAG_OFFSET, dup_fp);
            fwrite(data_buffer, 1, new_size, dup_fp);


            /* Calculate the new tag size */
            int offset = new_size - fsize;
            new_tag_size = tag_size + offset;

            break;
        }
        else
        {
            /* Copy the original frame unchanged */
            fwrite(frame_id, 1, ID_SIZE, dup_fp);
            fwrite(frame_size, 1, FRAME_SIZE, dup_fp);
            fwrite(flags, 1, FLAG_OFFSET, dup_fp);
            fwrite(data, 1, fsize, dup_fp);
        }


        /* Update total tag-body bytes processed */
        bytes_read += ID_SIZE + FRAME_SIZE + FLAG_OFFSET + fsize;
    }


    /* Copy remaining frames, padding and audio data */
    int ch;

    while((ch = fgetc(org_fp)) != EOF)
    {
        fputc(ch, dup_fp);
    }


    if(matchFound == 1)
    {
        /* Convert new tag size to synchsafe format */
        unsigned char synchsafe_size[4];

        synchsafe_size[0] = (new_tag_size >> 21) & 0x7F;
        synchsafe_size[1] = (new_tag_size >> 14) & 0x7F;
        synchsafe_size[2] = (new_tag_size >> 7)  & 0x7F;
        synchsafe_size[3] = new_tag_size & 0x7F;


        /* Update tag size in the copied header */
        fseek(dup_fp, 6, SEEK_SET);
        fwrite(synchsafe_size, 1, 4, dup_fp);


        /* Replace original file with edited file */
        remove(mp3->mp3_filename);
        rename("temp.mp3", mp3->mp3_filename);


        printf("\n");
        printf("========================================\n");
        printf("             MP3 TAG UPDATED\n");
        printf("========================================\n");
        printf("%-10s: %s\n",
               tag_id_to_label(mp3->tag_id), mp3->new_value);
        printf("----------------------------------------\n");
        printf("       %s changed successfully\n",
               tag_id_to_label(mp3->tag_id));
        printf("========================================\n\n");
    }
    else
    {
        printf("\n");
        printf("========================================\n");
        printf("             EDIT FAILED\n");
        printf("========================================\n");
        printf("No matching File identifier found!\n");
        printf("========================================\n\n");

        remove("temp.mp3");
    }


    /* Close both files */
    fclose(dup_fp);
    fclose(org_fp);
}