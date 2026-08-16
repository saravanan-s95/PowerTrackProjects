#ifndef REDO_UNDO_H
#define REDO_UNDO_H

#include "text_editor.h"


/* Stack operations */
void initializeStack(DynamicArrayStack *stack);

int push(DynamicArrayStack *stack, Action action);

Action pop(DynamicArrayStack *stack);

int isStackEmpty(DynamicArrayStack *stack);

void clearStack(DynamicArrayStack *stack);


/* Store an editor operation */
int recordAction(DynamicArrayStack *stack,const char *operation,const char *text,int cursorLine,int cursorPos);


/* Undo / Redo */
int undo(TextEditor *editor);

int redo(TextEditor *editor);

#endif