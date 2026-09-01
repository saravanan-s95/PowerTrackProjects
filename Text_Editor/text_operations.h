#ifndef TEXT_OPERATIONS_H
#define TEXT_OPERATIONS_H

#include "text_editor.h"

extern char clipboard[MAX_LINE_LENGTH];

Node *create_node(char *text);

int get_line_count(Node *head);

void insert_line(Node **head, Node **tail, char *text);

int delete_line(Node **head,Node **tail,int line_no,char *deleted_text);

void copy_text(Node *head,int line_no);

void cut_text(Node **head,Node **tail,int line_no);

void cut_text(Node **head,Node **tail,int line_no);

void paste_text(Node **head,Node **tail);

#endif