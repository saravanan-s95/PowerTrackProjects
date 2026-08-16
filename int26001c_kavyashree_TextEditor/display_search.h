#ifndef DISPLAY_SEARCH_H
#define DISPLAY_SEARCH_H

#include "text_editor.h"

/* Display */
void displayText(TextEditor *editor);

void showCursorPosition(TextEditor *editor);


/* Search */
int findText(TextEditor *editor, const char *search);

int replaceText(TextEditor *editor,
                const char *search,
                const char *replace);

#endif