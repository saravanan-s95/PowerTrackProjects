#ifndef TEXT_OPS_H
#define TEXT_OPS_H

#include"text_editor.h"
#include"undo_redo.h"

void insert_text(TextEditor*,char*,DynamicArrayStack*,DynamicArrayStack*);
void delete_chars(TextEditor*,char*,DynamicArrayStack*,DynamicArrayStack*);
void delete_line(TextEditor*,DynamicArrayStack*,DynamicArrayStack*);

void insert_text(TextEditor*text_editor,char*text,DynamicArrayStack*undo,DynamicArrayStack*redo);
void delete_chars(TextEditor*text_editor,char*str_num,DynamicArrayStack*undo,DynamicArrayStack*redo);
#endif