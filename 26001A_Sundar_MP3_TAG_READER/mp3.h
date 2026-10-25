#ifndef MP3_H
#define MP3_H

#include<stdio.h>
#include<string.h>

#include"types.h"

typedef struct TagReader
{
    //MP3 File name and MP3 file pointer
    char* mp3_file_name;
    FILE* fptr_mp3_file;

    //Version
    char version[8];

    //Buffer and size
    char buf[5];
    unsigned char size[4];
    unsigned int tag_size;

    // Total tag size is present in last 4 bytes of header and stored here
    unsigned int header_tag_size;

    //TAGS
    char title[100];
    char artist[100];
    char album[100];
    char year[5];
    char music[20];
    char comment[100];
    char mime_type[50];
    char picture_type[2];
    char description[100];

    //For editing 

    //Text to be edited
    char edit_text[100];
}tag;

//Check operation type
OperationType check_operation_type(char* argv);

//Viewing

//Read and validate view arguments
Status read_and_validate_view_arguments(char* argv[], tag* tagReader);

//Open the .mp3 file
Status open_mp3_file(tag* tagReader);

//View the .mp3 file
Status view_mp3_file(tag* tagReader);

//Check ID3 in file
Status check_id3(tag* tagReader);

//Check ID3 version in file
Status check_version(tag* tagReader);

//Get total tag size present in the file
Status get_total_tag_size(tag* tagReader);

//Read and store the TAGS into respective structure elements
Status read_and_store_tags(tag* tagReader);

//Display the stored structure elements
void display_mp3_contents(tag* tagReader);

//Editing

//Read and validate edit arguments
Status read_and_validate_edit_arguments(char* argv[], tag* tagReader);

//Edit the .mp3 file
Status edit_mp3_file(EditType op,tag* tagReader);

//Open the .mp3 file
Status open_mp3_file_for_editing(tag* tagReader);

//Check which TAG to be edited
EditType check_edit_type(char* argv);

//Edit a specific operation
Status edit_specific_operation(EditType op,tag* tagReader);

//Display the TAGS edited
void display_edited_tags(EditType op,tag* tagReader);

#endif