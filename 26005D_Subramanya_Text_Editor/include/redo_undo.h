#ifndef REDO_UNDO_H
#define REDO_UNDO_H

#include "text_editor.h"

/* Stack functions */
Status_e push_st(DynamicArrayStack *, Action);
Action pop_st(DynamicArrayStack *);

Status_e undo(TextEditor *, DynamicArrayStack *, DynamicArrayStack *);
Status_e redo(TextEditor *, DynamicArrayStack *, DynamicArrayStack *);

#endif