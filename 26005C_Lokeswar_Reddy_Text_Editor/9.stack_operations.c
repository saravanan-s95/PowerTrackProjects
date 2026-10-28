#include "header.h"


/*
Functions in this file:
    void initStack(DynArrStk *stk);
    void pushStack(DynArrStk *stk,const char *opr,const char *text,int line,int pos);
    int popStack(DynArrStk *stk, Action *action);
    void clearStack(DynArrStk *stk);
    void freeStack(DynArrStk *stk);
*/

void initStack(DynArrStk *stack)
{
    if (stack == NULL)
        return;

    stack->cap = 10;   //Initial Max capacity : 10
    stack->size = 0;
    stack->actions = malloc(sizeof(Action) * (size_t)stack->cap);

    if (stack->actions == NULL)
        stack->cap = 0;
}

void pushStack(DynArrStk *stack,const char *opr,const char *text,int line,int pos)
{
    if (stack == NULL || opr == NULL)
        return;


    if (stack->size >= stack->cap)
    {
        int newCapacity = stack->cap * 2;

        Action *newActions = realloc(stack->actions, sizeof(Action) * (size_t)newCapacity);
        if (newActions == NULL)
        {
            printf("Error: Stack resize failed.\n");
            return;
        }
        stack->actions = newActions;
        stack->cap = newCapacity;
    }

    stack->actions[stack->size].opr  = malloc(strlen(opr) + 1);  
    if (stack->actions[stack->size].opr == NULL)
    {
        return;
    }

    if(text != NULL)
    {
        stack->actions[stack->size].text = malloc(strlen(text) + 1);
        if (stack->actions[stack->size].text == NULL)
        {
            free(stack->actions[stack->size].opr);
            stack->actions[stack->size].opr = NULL;
            return;
        }
        strcpy(stack->actions[stack->size].text, text);
    }

    strcpy(stack->actions[stack->size].opr, opr);
    
    stack->actions[stack->size].cline = line;
    stack->actions[stack->size].cpos = pos;

    stack->size++;
}

int popStack(DynArrStk *stack, Action *action)
{
    if (stack == NULL || action == NULL || stack->size == 0)
        return 0;

    stack->size--;
    
    *action = stack->actions[stack->size];
    stack->actions[stack->size].opr = NULL;
    stack->actions[stack->size].text = NULL;
    return 1;
}

void clearStack(DynArrStk *stack)
{
    if (stack == NULL)
        return;
    
    for (int i = 0; i < stack->size; i++)
    {
        free(stack->actions[i].opr);
        free(stack->actions[i].text);
        stack->actions[i].opr = NULL;
        stack->actions[i].text = NULL;
    }
    stack->size = 0;
}

void freeStack(DynArrStk *stack)
{
    if (stack == NULL)
        return;
    
    clearStack(stack);
    free(stack->actions);
    stack->actions = NULL;
    stack->cap = 0;
}
