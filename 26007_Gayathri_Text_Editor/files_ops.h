#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "text_editor.h"

//Function to save the editor content to a file
void saveToFile(TextEditor *ed, const char *filename);

//Function to load the editor content to a file
int loadFromFile(TextEditor *ed, const char *filename);

#endif