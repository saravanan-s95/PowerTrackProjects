#include <iostream>
#include "../include/stack.h"

using namespace std;

// Constructor
Stack::Stack()
{
    top = -1;
}

// Check if stack is empty
bool Stack::isEmpty()
{
    return (top == -1);
}

// Check if stack is full
bool Stack::isFull()
{
    return (top == MAX - 1);
}

// Push element into stack
void Stack::push(int value)
{
    if (isFull())
    {
        cout << "\nStack Overflow! Cannot insert " << value << endl;
        return;
    }

    arr[++top] = value;
    cout << value << " pushed into stack." << endl;
}

// Pop element from stack
void Stack::pop()
{
    if (isEmpty())
    {
        cout << "\nStack Underflow! Stack is empty." << endl;
        return;
    }

    cout << arr[top] << " popped from stack." << endl;
    top--;
}

int Stack::popValue()
{
    if (isEmpty())
    {
        cout << "\nStack Underflow!" << endl;
        return -1;
    }

    return arr[top--];
}

// Peek top element
int Stack::peek()
{
    if (isEmpty())
    {
        cout << "\nStack is empty." << endl;
        return -1;
    }

    return arr[top];
}

// Display stack elements
void Stack::display()
{
    if (isEmpty())
    {
        cout << "\nStack is empty." << endl;
        return;
    }

    cout << "\nStack Elements (Top to Bottom):" << endl;

    for (int i = top; i >= 0; i--)
    {
        cout << arr[i] << endl;
    }
}