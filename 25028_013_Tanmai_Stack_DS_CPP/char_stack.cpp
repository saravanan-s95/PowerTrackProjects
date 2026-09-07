#include <iostream>
#include "../include/char_stack.h"

using namespace std;

// Constructor
CharStack::CharStack()
{
    top = -1;
}

// Check Empty
bool CharStack::isEmpty()
{
    return (top == -1);
}

// Check Full
bool CharStack::isFull()
{
    return (top == MAX - 1);
}

// Push
void CharStack::push(char ch)
{
    if (isFull())
    {
        cout << "Stack Overflow\n";
        return;
    }

    arr[++top] = ch;
}

// Pop
void CharStack::pop()
{
    if (isEmpty())
    {
        cout << "Stack Underflow\n";
        return;
    }

    top--;
}

// Peek
char CharStack::peek()
{
    if (isEmpty())
        return '\0';

    return arr[top];
}

// Display
void CharStack::display()
{
    if (isEmpty())
    {
        cout << "Stack Empty\n";
        return;
    }

    cout << "\nCharacter Stack\n";

    for (int i = top; i >= 0; i--)
        cout << arr[i] << " ";

    cout << endl;
}