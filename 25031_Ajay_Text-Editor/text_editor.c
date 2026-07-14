#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

Node *head = NULL;

void initialize_stack(Stack *s)
{
    s->top = -1;
}

int is_empty(Stack *s)
{
    return (s->top == -1);
}

int is_full(Stack *s)
{
    return (s->top == MAX_STACK - 1);
}

void push(Stack *s, Action act)
{
    if (is_full(s))
    {
        printf("Stack overflow\n");
        return;
    }
    s->actions[++s->top] = act;
}

// This returns action so that data can be used by undo or redo stack (Gives topmost action)
Action pop(Stack *s)
{
    Action empty = {"", "", -1};
    if (is_empty(s))
    {
        printf("Stack underflow\n");
        return empty;
    }
    return s->actions[s->top--];
}

// Initilisation of both undo and redo stack
Stack undo_stack = {.top = -1};
Stack redo_stack = {.top = -1};
