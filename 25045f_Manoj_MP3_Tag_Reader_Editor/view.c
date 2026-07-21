#include <stdio.h>
#include <string.h>
#include "view.h"
#include "common.h"


/* Read and validate view args from argv */
Status read_and_validate_view_args(char *argv[], ViewInfo *v_info){
    /*verify .mp3 extension*/
    char *ext = strrchr(argv[2], '.');
    if(strstr(ext , ".mp3") != NULL){
        v_info->mp3_file_name = argv[2];
    }else{
        printf("Invalid file format. Please provide an MP3 (.mp3) file.\n");
        return e_failure;
    }
    return e_success;
}


/* Perform viewing the metadata from mp3 file */
Status view_mp3_meta_data(ViewInfo *v_info){
    if(open_files(v_info) == e_success){
        if(read_and_validate_header(v_info) == e_success){
            if(read_frames(v_info) != e_success){
                printf("Error: Failed to read frames\n");
                return e_failure;
            }
        }else{
            printf("Error: Unable to read header\n");
            return e_failure;
        }
    }else{
        printf("Unable to open the file %s. Make sure it is present in the current working directory.\n" , v_info->mp3_file_name);
        return e_failure;
    }
    return e_success;
}

Status open_files(ViewInfo *v_info){
    v_info->fptr_file_mp3 = fopen(v_info->mp3_file_name , "rb");

    if(v_info->fptr_file_mp3 == NULL){
        return e_failure;
    }

    return e_success;
}

/*Validate header*/
Status read_and_validate_header(ViewInfo *v_info){
    char header[HEADER_SIZE];
   
    if(fread(header , HEADER_SIZE , 1 , v_info->fptr_file_mp3) == HEADER_SIZE){
        if((strncmp(header , "ID3" , 3)) != 0){
            printf("Error: Not a valid MP3 file with ID3 tag\n");
            return e_failure;
        }
    }

    /* get metadata size to know till where we need to read to get metadata*/
    v_info->metadata_size = (((unsigned char)header[6] << 21) | ((unsigned char)header[7] << 14) | ((unsigned char)header[8] << 7) | ((unsigned char)header[9]));
    //printf("ID3 tag found\n");  

    return e_success;
}

/*Read frames*/
Status read_frames(ViewInfo *v_info){
    int bytes_read = 0;
    strcpy(v_info->title , "--");
    strcpy(v_info->artist , "--");
    strcpy(v_info->album , "--");
    strcpy(v_info->year , "--");
    strcpy(v_info->genre , "--");
    strcpy(v_info->comment , "--");
    while(bytes_read < v_info->metadata_size){
        /*Read frame ID*/
        fread(v_info->frame_ID , FRAME_ID_SIZE , 1 , v_info->fptr_file_mp3);
        v_info->frame_ID[FRAME_ID_SIZE] = '\0';
        bytes_read = bytes_read + FRAME_ID_SIZE; 
       // printf("ID %s\n", v_info->frame_ID);
        
        /*Read frame size*/
        char size_bytes[FRAME_SIZE_BYTES];
        fread(size_bytes , FRAME_SIZE_BYTES , 1 , v_info->fptr_file_mp3);
        bytes_read = bytes_read + FRAME_SIZE_BYTES;
        v_info->frame_size = (((unsigned char)size_bytes[0] << 24) | ((unsigned char)size_bytes[1] << 16) | ((unsigned char)size_bytes[2] << 8) | ((unsigned char)size_bytes[3]));
      //  printf("Size %u\n", v_info->frame_size);
        
        /*Skipping flags*/
        fseek(v_info->fptr_file_mp3 , FRAME_FLAGS_SIZE , SEEK_CUR);
        bytes_read = bytes_read + FRAME_FLAGS_SIZE;
        
        char data[v_info->frame_size + 1];
        fread(data , v_info->frame_size , 1 , v_info->fptr_file_mp3);
        data[v_info->frame_size] = '\0';
        bytes_read = bytes_read + v_info->frame_size;

        if(strcmp(v_info->frame_ID, TITLE) == 0 && v_info->frame_size > 0){
            strcpy(v_info->title , data + 1);
        }else if(strcmp(v_info->frame_ID, ARTIST) == 0 && v_info->frame_size > 0){
            strcpy(v_info->artist, data + 1);
        }else if(strcmp(v_info->frame_ID, ALBUM) == 0 && v_info->frame_size > 0){
            strcpy(v_info->album, data + 1);
        }else if(strcmp(v_info->frame_ID, YEAR) == 0 && v_info->frame_size > 0){
            strcpy(v_info->year, data + 1);
        }else if(strcmp(v_info->frame_ID, GENRE) == 0 && v_info->frame_size > 0){
            strcpy(v_info->genre, data + 1);
        }else if(strcmp(v_info->frame_ID, COMMENT) == 0 && v_info->frame_size > 0){
            strcpy(v_info->comment, data + 5);
        }
    }
    return e_success;
}


/*Display frame*/
void display_frames(ViewInfo *v_info){
    printf("  %-8s : %s\n", "Title",   v_info->title);
    printf("  %-8s : %s\n", "Artist",  v_info->artist);
    printf("  %-8s : %s\n", "Album",   v_info->album);
    printf("  %-8s : %s\n", "Year",    v_info->year);
    printf("  %-8s : %s\n", "Genre",   v_info->genre);
    printf("  %-8s : %s\n", "Comment", v_info->comment);
}