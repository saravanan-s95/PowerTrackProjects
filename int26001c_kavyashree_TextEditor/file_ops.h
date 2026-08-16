#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "text_editor.h"

int openFile(TextEditor *editor, const char *filename);

int saveFile(TextEditor *editor, const char *filename);

int closeFile(TextEditor *editor);

#endif