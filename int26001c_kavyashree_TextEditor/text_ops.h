#ifndef TEXT_OPS_H
#define TEXT_OPS_H

#include "text_editor.h"

//text operations
int insertText(TextEditor *editor, const char *text);

int deleteCharacters(TextEditor *editor, int count);

int copyText(TextEditor *editor, int count);

int cutText(TextEditor *editor, int count);

int pasteText(TextEditor *editor);

int deleteLine(TextEditor *editor);

#endif