#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "text_editor.h"

int save_file(Node *head, const char *filename);
int open_file(Node **head, Node **tail, const char *filename);

#endif