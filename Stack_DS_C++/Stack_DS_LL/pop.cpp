#include "stack.h"

int Stack :: Pop()
{
    if(top == nullptr)
    {
        return FAILURE;
    }
    
    Node* temp = top;
    
    top = temp->link;
    delete(temp);
    return SUCCESS;
}