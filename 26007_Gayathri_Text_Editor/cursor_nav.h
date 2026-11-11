#ifndef CURSOR_NAV_H
#define CURSOR_NAV_H
#include "text_editor.h"

//function to move cursor one line up
void moveUp(TextEditor *ed);

//function to move cursor one line down
void moveDown(TextEditor *ed);

//function to move cursor one position to left
void moveRight(TextEditor *ed);

//function to move cursor one position to right
void moveLeft(TextEditor *ed);

#endif