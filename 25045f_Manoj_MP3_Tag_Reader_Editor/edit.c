#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"
#include "common.h"

/* Read and validate edit args from argv */
Status read_and_validate_edit_args(char *argv[], EditInfo *e_info){
    /*validate edit option*/
    if((strcmp(argv[2] , EDIT_SONG_TITLE) == 0) || (strcmp(argv[2] , EDIT_ARTIST_NAME) == 0) || (strcmp(argv[2] , EDIT_ALBUM_NAME) == 0) || (strcmp(argv[2] , EDIT_YEAR) == 0) || (strcmp(argv[2] , EDIT_GENRE) == 0) || (strcmp(argv[2] , EDIT_COMMENT) == 0)){
         e_info->edit_option = argv[2];
    }else{
        printf("Please provide a valid edit option, for more details, use:   ./a.out --help\n");
        return e_failure;
    }
    
    /*store new text*/
    e_info->new_edit_text = argv[3];
        
    /*validate .mp3 extension*/
    char *ext = strrchr(argv[4], '.');
    if((strstr(ext , ".mp3")) != NULL){
        e_info->mp3_file_name = argv[4];
    }else{
        printf("Invalid file format. Please provide an MP3 (.mp3) file.\n");
        return e_failure;
    }
    return e_success;
}

/*Edit mp3 meta data*/
Status edit_mp3_meta_data(EditInfo *e_info){
    if(open_files_to_edit(e_info) == e_success){
        if(copy_header(e_info) == e_success){
            if(edit_frames(e_info) == e_success){
                if(copy_remaining_data(e_info) == e_success){
                    if(close_files(e_info) == e_success){
                        if(update_original_file(e_info) != e_success){
                                return e_failure;
                        }
                    }
                }else{
                    return e_failure;
                }
            }else{
                return e_failure;
            }
        }else{
            return e_failure;
        }
    }else{
        printf("Unable to open the file %s. Make sure it is present in the current working directory.\n" , e_info->mp3_file_name);
        return e_failure;
    }
    return e_success;
}

/* File pointer */
Status open_files_to_edit(EditInfo *e_Info){
    e_Info->fptr_src_file_mp3 = fopen(e_Info->mp3_file_name, "rb");
    
    if(e_Info->fptr_src_file_mp3 == NULL){
        return e_failure;
    }
    
    /*create temp file for writing data*/
    e_Info->fptr_dest_file_mp3 = fopen("temp.mp3", "wb");
    
    if(e_Info->fptr_dest_file_mp3 == NULL){
        // printf("Unable to edit file\n");
        return e_failure;
    }
    return e_success;
}

/*Copy header*/
Status copy_header(EditInfo *e_Info){
    /*read header from source file*/
    char header[HEADER_SIZE];
    if((fread(header , 1 , HEADER_SIZE, e_Info->fptr_src_file_mp3)) != HEADER_SIZE){
        return e_failure;
    }
    fwrite(header ,1 , HEADER_SIZE , e_Info->fptr_dest_file_mp3);

    /*get metadata size*/
    e_Info->metadata_size = (((unsigned char)header[6] << 21) | ((unsigned char)header[7] << 14) | ((unsigned char)header[8] << 7) | ((unsigned char)header[9]));
    
    //Store ID3 version from bytes 3 and 4
    e_Info->version_major = (unsigned char)header[3];
    e_Info->version_minor = (unsigned char)header[4];
    return e_success;
}


/*Edit frames*/
Status edit_frames(EditInfo *e_info){
    int bytes_read = HEADER_SIZE;
    int found = 0;
    /*store edit option frame*/
    char target[5];
    if(strcmp(e_info->edit_option , EDIT_SONG_TITLE) == 0)
        strcpy(target , TITLE);
    else if(strcmp(e_info->edit_option , EDIT_ARTIST_NAME) == 0)
        strcpy(target , ARTIST);
    else if(strcmp(e_info->edit_option , EDIT_ALBUM_NAME) == 0)
        strcpy(target , ALBUM);
    else if(strcmp(e_info->edit_option , EDIT_YEAR) == 0)
        strcpy(target , YEAR);
    else if (strcmp(e_info->edit_option, EDIT_GENRE) == 0)
        strcpy(target, GENRE);
    else if (strcmp(e_info->edit_option, EDIT_COMMENT) == 0)
        strcpy(target, COMMENT);
    
    while(bytes_read < e_info->metadata_size + HEADER_SIZE){
        /*Read Frame ID*/
        fread(e_info->frame_ID , 1 , FRAME_ID_SIZE , e_info->fptr_src_file_mp3);
        e_info->frame_ID[FRAME_ID_SIZE] = '\0';
        bytes_read = bytes_read + FRAME_ID_SIZE;
        
        //Detect padding frame ID with all zeros
        if (e_info->frame_ID[0] == '\0') {
            // Write the zeros
            fwrite(e_info->frame_ID, 1, FRAME_ID_SIZE, e_info->fptr_dest_file_mp3);
            // Copy remaining tag bytes (padding)
            int remaining = (e_info->metadata_size + HEADER_SIZE) - bytes_read;
            for (int i = 0; i < remaining; i++) {
                char ch;
                fread(&ch, 1, 1, e_info->fptr_src_file_mp3);
                fwrite(&ch, 1, 1, e_info->fptr_dest_file_mp3);
            }
            bytes_read += remaining;
            break;
        }
        
        /*Read Frame Size*/
        char size_bytes[FRAME_SIZE_BYTES];
        fread(size_bytes , 1 , FRAME_SIZE_BYTES , e_info->fptr_src_file_mp3);
        // e_info->frame_size = (((unsigned char)size_bytes[0] << 24) | ((unsigned char)size_bytes[1] << 16) | ((unsigned char)size_bytes[2] << 8) | ((unsigned char)size_bytes[3]));
        bytes_read = bytes_read + FRAME_SIZE_BYTES;
         // frame size according to version 
        if (e_info->version_major == 4) {
            e_info->frame_size = (((unsigned char)size_bytes[0] << 21) | ((unsigned char)size_bytes[1] << 14) | ((unsigned char)size_bytes[2] << 7)  | ((unsigned char)size_bytes[3]));
        } else {
            e_info->frame_size = (((unsigned char)size_bytes[0] << 24) | ((unsigned char)size_bytes[1] << 16) | ((unsigned char)size_bytes[2] << 8)  | ((unsigned char)size_bytes[3]));
        }
        
        /*Skit Flag bytes*/
        fread(e_info->frame_flags , 1 , FRAME_FLAGS_SIZE , e_info->fptr_src_file_mp3);
        bytes_read = bytes_read + FRAME_FLAGS_SIZE;
        
        /*Read data*/
        char *data = malloc(e_info->frame_size + 1);
        if(data == NULL){
            return e_failure;
        }
        fread(data , 1 , e_info->frame_size , e_info->fptr_src_file_mp3);
        data[e_info->frame_size] = '\0';
        bytes_read = bytes_read + e_info->frame_size;
        
        
        /*Compare*/
        if(strcmp(e_info->frame_ID , target) == 0){
            if(strlen(e_info->new_edit_text) > e_info->frame_size - 1)
            {
                printf("New text too large. Maximum allowed size is %d characters\n", e_info->frame_size - 1);

                free(data);
                return e_failure;
            }
            found = 1;
            /*frame ID*/
            fwrite(e_info->frame_ID , 1 , FRAME_ID_SIZE , e_info->fptr_dest_file_mp3);
            
            /*write original frame size*/
            fwrite(size_bytes, 1, FRAME_SIZE_BYTES, e_info->fptr_dest_file_mp3);
            
            /*write flags*/
            fwrite(e_info->frame_flags , 1 , FRAME_FLAGS_SIZE, e_info->fptr_dest_file_mp3);
            
            /*write new text*/
            char encoding = data[0];
            fwrite(&encoding , 1 , 1 , e_info->fptr_dest_file_mp3);

            /* create empty buffer */
            char *temp = calloc(1, e_info->frame_size - 1);

            if(temp == NULL)
            {
                free(data);
                return e_failure;
            }

            /* copy new text */
            strncpy(temp, e_info->new_edit_text, e_info->frame_size - 1);

            /* write FULL original frame size */
            fwrite(temp, 1, e_info->frame_size - 1, e_info->fptr_dest_file_mp3);

            free(temp);
        }else{
            fwrite(e_info->frame_ID, 1, FRAME_ID_SIZE, e_info->fptr_dest_file_mp3);
            fwrite(size_bytes, 1, FRAME_SIZE_BYTES, e_info->fptr_dest_file_mp3);
            fwrite(e_info->frame_flags, 1, FRAME_FLAGS_SIZE, e_info->fptr_dest_file_mp3);
            fwrite(data, 1, e_info->frame_size, e_info->fptr_dest_file_mp3);
        }
        free(data);
    }
     if(found == 0)
    {
        printf("Selected tag not found in MP3 file\n");
        fclose(e_info->fptr_src_file_mp3);
        fclose(e_info->fptr_dest_file_mp3);
        remove("temp.mp3");
        return e_failure;
    }

    return e_success;
}

/*Copy remaining data*/
Status copy_remaining_data(EditInfo *e_info){
    int buffer;

     while ((buffer = fgetc(e_info->fptr_src_file_mp3)) != EOF)
    {
        fputc(buffer, e_info->fptr_dest_file_mp3);
    }
    return e_success;
}

/*Close files*/
Status close_files(EditInfo *e_info)
{
    fclose(e_info->fptr_src_file_mp3);
    fclose(e_info->fptr_dest_file_mp3);

    return e_success;
}

/*Update original file*/
Status update_original_file(EditInfo *e_info){
    remove(e_info->mp3_file_name);
    rename("temp.mp3", e_info->mp3_file_name);

    return e_success;
}
