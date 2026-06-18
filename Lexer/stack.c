#include "stack.h"
#include <stdio.h>

// To initialize the stack
void init_stack(Stack *stk){
    stk->top = -1;
}

// To push a character into stack
void push(Stack *stk, char ch){
    stk->top++;

    if(stk->top == MAX_STACK_SIZE){
        printf("\nStack Overflow.\n");
        return;
    }

    // Has enough space
    stk->values[stk->top] = ch;
}



// To pop a character from the stack
char pop(Stack *stk){

    if(stk->top == -1){
        printf("\nStack Underflow.\n");
        return '\0';
    }

    return stk->values[stk->top--];
}


// Top of the stack
char top(Stack *stk){
    return stk->values[stk->top];
}