#ifndef TEXT_OPS_H
#define TEXT_OPS_H

#include "text_editor.h"

int insert_char(Node *cursor, int position, char ch);
int delete_char(Node *cursor, int position);

extern char *clipboard;

int copy_line(Node *cursor);
int cut_line(Node **head, Node **tail, Node *cursor);
int paste_line(Node **head, Node **tail, int line_number);

#endif
