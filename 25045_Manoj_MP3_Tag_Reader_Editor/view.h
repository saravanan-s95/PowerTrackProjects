#ifndef VIEW_H
#define VIEW_H

#include "types.h"

#define MAX_FILE_SUFFIX 5
#define HEADER_SIZE 10 // Size of the mp3 filer header contains version and metadata size
/*Frame Header for each frames*/
#define FRAME_ID_SIZE 4
#define FRAME_SIZE_BYTES 4
#define FRAME_FLAGS_SIZE 2


typedef struct _ViewInfo
{
    char *mp3_file_name;
    FILE *fptr_file_mp3;
    
    uint metadata_size; //based on this size we get to know till where we need to read
    char frame_ID[5];
    uint frame_size;
    
    char title[100];
    char artist[100];
    char album[100];
    char year[5];
    char genre[50];
    char comment[200];
    
}ViewInfo;

/* View function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate view args from argv */
Status read_and_validate_view_args(char *argv[], ViewInfo *v_info);

/* Perform Viewing */
Status view_mp3_meta_data(ViewInfo *v_Info);

/* Get File pointers for i/p file */
Status open_files(ViewInfo *v_Info);

/*Validate header*/
Status read_and_validate_header(ViewInfo *v_info);

/*Read frames*/
Status read_frames(ViewInfo *v_info);

/*Display frame*/
void display_frames(ViewInfo *v_info);


#endif