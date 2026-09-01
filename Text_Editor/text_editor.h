#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define STACK_SIZE 100

/* Node for Doubly Linked List */

typedef struct node
{
    char line[MAX_LINE_LENGTH];

    struct node *prev;
    struct node *next;

}Node;


/* Action for Undo/Redo */

typedef struct
{
    char operation[20];

    char text[MAX_LINE_LENGTH];

    int line_no;

}Action;


/* Stack */

typedef struct
{
    Action data[STACK_SIZE];

    int top;

}Stack;


/* Cursor */

typedef struct
{
    Node *current;

    int row;

    int col;

}Cursor;

#endif