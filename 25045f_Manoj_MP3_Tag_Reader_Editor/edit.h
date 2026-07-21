#ifndef EDIT_H
#define EDIT_H

#include "types.h"

#define MAX_FILE_SUFFIX 5
#define HEADER_SIZE 10 // Size of the mp3 filer header contains version and metadata size
/*Frame Header for each frames*/
#define FRAME_ID_SIZE 4
#define FRAME_SIZE_BYTES 4
#define FRAME_FLAGS_SIZE 2
#define EDIT_SONG_TITLE "-t"
#define EDIT_ARTIST_NAME "-a"
#define EDIT_ALBUM_NAME "-A"
#define EDIT_YEAR "-y"
#define EDIT_GENRE "-m"
#define EDIT_COMMENT "-c"


typedef struct  _EditInfo
{
    char *mp3_file_name;
    FILE *fptr_src_file_mp3;
    
    FILE *fptr_dest_file_mp3;
    char *edit_option;
    char *new_edit_text;
    char new_size_bytes[FRAME_SIZE_BYTES];
    unsigned char version_major;
    unsigned char version_minor;
    
    uint metadata_size;
    char frame_ID[5];
    uint frame_size;
    char frame_flags[FRAME_FLAGS_SIZE];
    
}EditInfo;

/* View function prototype */

/* Read and validate edit args from argv */
Status read_and_validate_edit_args(char *argv[], EditInfo *e_info);

/*Edit mp3 meta data*/
Status edit_mp3_meta_data(EditInfo *e_info);

/* File pointer */
Status open_files_to_edit(EditInfo *e_Info);

/*Copy header*/
Status copy_header(EditInfo *e_Info);

/*Edit frames*/
Status edit_frames(EditInfo *e_info);

/*Copy remaining data*/
Status copy_remaining_data(EditInfo *e_info);

/*Display frame*/
Status close_files(EditInfo *e_info);

/*Update original file*/
Status update_original_file(EditInfo *e_info);

#endif