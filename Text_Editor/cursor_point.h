#ifndef CURSOR_NAVIGATIONS_H
#define CURSOR_NAVIGATIONS_H

#include "text_editor.h"

void init_cursor(Cursor *cursor, Node *head);

void move_up(Cursor *cursor);

void move_down(Cursor *cursor);

void show_cursor(Cursor *cursor);

#endif