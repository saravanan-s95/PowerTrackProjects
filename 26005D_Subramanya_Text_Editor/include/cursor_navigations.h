#ifndef CURSOR_NAVIGATIONS_H
#define CURSOR_NAVIGATIONS_H

#include "text_editor.h"

#define min(a,b) ((a) < (b) ? (a) : (b))

/* Cursor Nagivation Functions */
Status_e moveCursorRight(TextEditor *);
Status_e moveCursorLeft(TextEditor *);
Status_e moveCursorDown(TextEditor *);
Status_e moveCursorUp(TextEditor *);
Status_e moveCursorToLine(TextEditor *editor, int targetLine);
Status_e moveCursorStart(TextEditor *);
Status_e moveCursorEnd(TextEditor *);

#endif