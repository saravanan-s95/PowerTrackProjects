#include "stack.h"

int Stack :: Peek()
{
    if(top == nullptr)
    {
        return FAILURE;
    }
    
    return top->data;
}