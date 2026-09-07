#include <iostream>
#include "../include/linked_stack.h"

using namespace std;

// Constructor
LinkedStack::LinkedStack()
{
    top = NULL;
}

// Check if stack is empty
bool LinkedStack::isEmpty()
{
    return (top == NULL);
}

// Push
void LinkedStack::push(int value)
{
    Node *newNode = new Node;

    newNode->data = value;
    newNode->next = top;
    top = newNode;

    cout << value << " pushed into stack." << endl;
}

// Pop
void LinkedStack::pop()
{
    if (isEmpty())
    {
        cout << "Stack Underflow!" << endl;
        return;
    }

    Node *temp = top;

    cout << top->data << " popped from stack." << endl;

    top = top->next;

    delete temp;
}

// Peek
int LinkedStack::peek()
{
    if (isEmpty())
    {
        cout << "Stack is empty." << endl;
        return -1;
    }

    return top->data;
}

// Display
void LinkedStack::display()
{
    if (isEmpty())
    {
        cout << "Stack is empty." << endl;
        return;
    }

    cout << "\nStack Elements (Top to Bottom):" << endl;

    Node *temp = top;

    while (temp != NULL)
    {
        cout << temp->data << endl;
        temp = temp->next;
    }
}

// Destructor
LinkedStack::~LinkedStack()
{
    while (!isEmpty())
    {
        pop();
    }
}