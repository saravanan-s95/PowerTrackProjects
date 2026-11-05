#ifndef REDO_UNDO_H
#define REDO_UNDO_H

#include "text_editor.h"

typedef struct
{
    char *operation;
    char *text;
    int cursorLine;
    int cursorPos;
} Action;

typedef struct
{
    Action *actions;
    int size;
    int capacity;
} DynamicArrayStack;

void initStack(DynamicArrayStack *stack);
int pushAction(DynamicArrayStack *stack, const char *operation,
               const char *text, int cursorLine, int cursorPos);
int popAction(DynamicArrayStack *stack, Action *action);
void clearStack(DynamicArrayStack *stack);
void undoAction(TextEditor *editor);
void redoAction(TextEditor *editor);

#endif
