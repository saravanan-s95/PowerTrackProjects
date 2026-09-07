#include <iostream>
#include "../include/string_stack.h"

using namespace std;

StringStack::StringStack()
{
    top = -1;
}

bool StringStack::isEmpty()
{
    return top == -1;
}

bool StringStack::isFull()
{
    return top == MAX_HISTORY - 1;
}

void StringStack::push(string page)
{
    if (isFull())
    {
        cout << "\nHistory Full!\n";
        return;
    }

    arr[++top] = page;
}

void StringStack::pop()
{
    if (isEmpty())
        return;

    top--;
}

string StringStack::popValue()
{
    if (isEmpty())
        return "";

    return arr[top--];
}

string StringStack::peek()
{
    if (isEmpty())
        return "";

    return arr[top];
}

void StringStack::display()
{
    if (isEmpty())
    {
        cout << "No History Available.\n";
        return;
    }

    for (int i = top; i >= 0; i--)
        cout << arr[i] << endl;
}