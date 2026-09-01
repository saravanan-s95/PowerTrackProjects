#ifndef STACK_H
#define STACK_H

#include "text_editor.h"

void init_stack(Stack *s);

int isempty(Stack *s);

int isfull(Stack *s);

void push(Stack *s, Action action);

Action *pop(Stack *s);

#endif