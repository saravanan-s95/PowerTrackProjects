#ifndef DISPLAY_SEARCH_H
#define DISPLAY_SEARCH_H

#include "text_editor.h"

void displayText(TextEditor *editor);
void showCursorPosition(TextEditor *editor);
int findText(TextEditor *editor, const char *pattern);
int replaceText(TextEditor *editor, const char *pattern, const char *replacement);

#endif
