#include "stack.h"

int Stack :: Push(data_t data)
{
    Node* ptr = new Node(data,nullptr);
    
    ptr->link = top;
    top = ptr;
    return SUCCESS;
}