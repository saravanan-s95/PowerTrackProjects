#ifndef LINKED_STACK_H
#define LINKED_STACK_H

class Node
{
public:
    int data;
    Node *next;
};

class LinkedStack
{
private:
    Node *top;

public:
    LinkedStack();

    bool isEmpty();

    void push(int value);
    void pop();
    int peek();
    void display();

    ~LinkedStack();
};

#endif