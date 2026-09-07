#ifndef STACK_H
#define STACK_H

#define MAX 100

class Stack
{
private:
    int arr[MAX];
    int top;

public:
    // Constructor
    Stack();

    // Stack operations
    bool isEmpty();
    bool isFull();
    void push(int value);
    void pop();
    int popValue();
    int peek();
    void display();
};

#endif