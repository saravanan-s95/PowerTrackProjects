#ifndef TEXT_OPS_H
#define TEXT_OPS_H

#include "text_editor.h"

struct Node
{
    char *text;
    struct Node *prev;
    struct Node *next;
};

void insertText(TextEditor *editor, const char *text);
void deleteCharacters(TextEditor *editor, int count);
void copyText(TextEditor *editor, int count);
void cutText(TextEditor *editor, int count);
void pasteText(TextEditor *editor);
void deleteLine(TextEditor *editor);
void moveCursor(TextEditor *editor, int line, int pos);
void displayText(TextEditor *editor);

#endif