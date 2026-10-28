#include "header.h"

/*
Functions in this file:
    void initEditor(TxtEdtr *editor);
    void freeEditor(TxtEdtr *editor);
*/

void initEditor(TxtEdtr *editor)
{
    if (editor == NULL)
        return;

    editor->head = NULL;
    editor->tail = NULL;
    editor->cursor = NULL;
    editor->cline = 1;
    editor->cpos = 0;
}

void freeEditor(TxtEdtr *editor)
{
    Node *current;
    Node *next;

    if (editor == NULL)
        return;

    current = editor->head;
    while (current != NULL)
    {
        next = current->nxt;
        free(current->txt);
        free(current);
        current = next;
    }

    initEditor(editor);
}
