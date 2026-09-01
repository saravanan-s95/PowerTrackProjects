#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "text_editor.h"

void open_file(Node **head, Node **tail, char *filename);

void save_file(Node *head, char *filename);

void close_file(void);

#endif