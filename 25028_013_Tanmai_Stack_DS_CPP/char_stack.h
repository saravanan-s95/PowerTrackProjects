#ifndef CHAR_STACK_H
#define CHAR_STACK_H

#define MAX 100

class CharStack
{
private:
    char arr[MAX];
    int top;

public:
    CharStack();

    bool isEmpty();
    bool isFull();

    void push(char ch);
    void pop();

    char peek();

    void display();
};

#endif