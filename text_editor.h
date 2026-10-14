#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

struct Node
{
    char data[SIZE];
    struct Node *next;
};

void insert_text(struct Node **head);
int search_text(struct Node *head, char *str);
void display_text(struct Node *head);
void delete_text(struct Node **head);
void update_text(struct Node *head);
void save_text(struct Node *head);
void load_text(struct Node **head);

#endif