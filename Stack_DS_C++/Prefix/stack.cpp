#include "main.h"

void Stack :: push(int data)
{
    stack[++top] = data;
}

void Stack :: pop()
{
	if(top != -1)
    {
        --top;
    }
}

int Stack :: peek()
{
	if(top != -1)
	{
		return stack[top];
	}
	return -1;
}

int Stack :: priority(char opr)
{
    if(opr == '+' || opr == '-')
    {
        return 1;
    }
    else if(opr == '*' || opr == '/')
    {
        return 2;
    }
    else
    {
        return 0;
    }
}