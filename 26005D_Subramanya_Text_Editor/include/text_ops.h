#ifndef TEXT_OPS_H
#define TEXT_OPS_H

#include "text_editor.h"


Status_e copyText(TextEditor *editor, int startPos, int endPos);
Status_e cutText(TextEditor *editor, int startPos, int endPos, DynamicArrayStack *undoStack);
Status_e pasteText(TextEditor *editor, DynamicArrayStack *undoStack);

#endif