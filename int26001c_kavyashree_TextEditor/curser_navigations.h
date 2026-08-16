#ifndef CURSER_NAVIGATIONS_H
#define CURSER_NAVIGATIONS_H

#include "text_editor.h"

void moveCursorUp(TextEditor *editor);

void moveCursorDown(TextEditor *editor);

void moveCursorLeft(TextEditor *editor);

void moveCursorRight(TextEditor *editor);

void jumpToStartOfFile(TextEditor *editor);

void jumpToEndOfFile(TextEditor *editor);

void jumpToStartOfLine(TextEditor *editor);

void jumpToEndOfLine(TextEditor *editor);

#endif