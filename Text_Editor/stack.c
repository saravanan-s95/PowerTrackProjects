#include "text_editor.h"
#include "stack.h"

void init_stack(Stack *s)
{
    s->top = -1;
}

int isempty(Stack *s)
{
    if(s->top == -1)
    {
        return 1;
    }

    return 0;
}


int isfull(Stack *s)
{
    if(s->top == STACK_SIZE - 1)
    {
        return 1;
    }

    return 0;
}


void push(Stack *s, Action action)
{
    if(isfull(s))
    {
        printf("Stack Overflow\n");
        return;
    }

    s->top++;

    s->data[s->top] = action;
}


Action *pop(Stack *s)
{
    if(isempty(s))
    {
        return NULL;
    }

    Action *temp = malloc(sizeof(Action));

    if(temp == NULL)
    {
        return NULL;
    }

    *temp = s->data[s->top];

    s->top--;

    return temp;
}

