#include "header.h"

/*
Functions in this file:
    void moveCursorUp(TxtEdtr *editor);
    void moveCursorDown(TxtEdtr *editor);
    void moveCursorLeft(TxtEdtr *editor);
    void moveCursorRight(TxtEdtr *editor);

    void jumpToStartOfFile(TxtEdtr *editor);
    void jumpToEndOfFile(TxtEdtr *editor);

    void jumpToStartOfLine(TxtEdtr *editor);
    void jumpToEndOfLine(TxtEdtr *editor);
*/

void moveCursorUp(TxtEdtr *editor) 
{
    if (editor->cursor == NULL)
        return;

    if (editor->cursor->prv != NULL)
    {
        editor->cursor = editor->cursor->prv;

        editor->cline--;

        if (editor->cpos >(int)strlen(editor->cursor->txt))
            editor->cpos = strlen(editor->cursor->txt);
    }
}


void moveCursorDown(TxtEdtr *editor) 
{
    if (editor->cursor == NULL)
        return;

    if (editor->cursor->nxt != NULL)
    {
        editor->cursor = editor->cursor->nxt;

        editor->cline++;

        if (editor->cpos >(int)strlen(editor->cursor->txt))
            editor->cpos = strlen(editor->cursor->txt);
    }
}


void moveCursorLeft(TxtEdtr *editor)
{
    if (editor->cursor == NULL)
        return;

    if (editor->cpos > 0)
        editor->cpos--;
}


void moveCursorRight(TxtEdtr *editor)
{
    if (editor->cursor == NULL)
        return;

    int length = strlen(editor->cursor->txt);

    if (editor->cpos < length)
        editor->cpos++;   
}


void jumpToStartOfFile(TxtEdtr *editor)
{
    if (editor->head == NULL)
        return;
    
    editor->cursor = editor->head;
    editor->cline = 1;
    editor->cpos = 0;
}


void jumpToEndOfFile(TxtEdtr *editor)
{
    if (editor->tail == NULL)
        return;
    
    editor->cursor = editor->tail;

    editor->cline = 1;

    Node *current = editor->head;
    while (current != editor->tail)
    {
        editor->cline++;
        current = current->nxt;
    }
    
    editor->cpos = strlen(editor->cursor->txt);
}


void jumpToStartOfLine(TxtEdtr *editor)
{
    if (editor->cursor == NULL)
        return;

    editor->cpos = 0;
}


void jumpToEndOfLine(TxtEdtr *editor)
{
    if (editor->cursor == NULL)
        return;

    editor->cpos = strlen(editor->cursor->txt);
}