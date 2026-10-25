#ifndef FILE_OPS
#define FILE_OPS

#include "text_editor.h"
#include "undo_redo.h"

typedef struct 
{
    FILE* fp; //File pointer
    char filename[100]; //Name of the file
}File;

/* Validate file name */
int validate_file_name(File* file);

/* Open and load file contents into text editor */
int open_and_load_file(TextEditor* editor,DynamicArrayStack* undostack,File* file);

/* Save to file */
int save_file(TextEditor editor,File* file);

#endif