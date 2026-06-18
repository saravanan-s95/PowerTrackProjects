#ifndef STACK_H
#define STACK_H

#define MAX_STACK_SIZE  100

/* This is a simple stack implementation with push and pop operations.*/
typedef struct {
    char values[100];
    int top;
}Stack;

// To initialize the stack
void init_stack(Stack *stk);

// To push a character into stack
void push(Stack *stk, char ch);

// To pop a character from the stack
char pop(Stack *stk);

// Top of the stack
char top(Stack *stk);

#endif