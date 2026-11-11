#ifndef TEXT_OPS_H
#define TEXT_OPS_H
#include "text_editor.h"

//Function to insert a single character at the current cursor pos
void insertchar(TextEditor *ed, char ch);

//Function to delete n characters before the cursor
void deletechars(TextEditor*ed, int n);

//Function to insert newline in editor
void insertNewLine(TextEditor *ed);

#endif