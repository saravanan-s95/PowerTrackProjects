#ifndef REDO_UNDO_H
#define REDO_UNDO_H

#include "text_editor.h"
#include "text_operations.h"
#include "stack.h"

void undo(Node **head, Node **tail, Stack *undoStack, Stack *redoStack);

void redo(Node **head, Node **tail, Stack *undoStack, Stack *redoStack);

#endif