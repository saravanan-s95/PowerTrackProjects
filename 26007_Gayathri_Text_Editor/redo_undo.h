#ifndef REDO_UNDO_H
#define REDO_UNDO_H

#include "text_editor.h"


// Function to initialize the undo / redo stack
void initStack(DynamicArrayStack *stack);

// Function to push a new operation onto the stack (undo or redo)
void pushOperation(DynamicArrayStack *stack, const char *op, const char *txt, int line, int col);

// Function to clear the redo stack
void clearRedo(TextEditor *ed);

// Function to perform undo
void performUndo(TextEditor *ed);

// Function to perform redo
void performRedo(TextEditor *ed);

#endif