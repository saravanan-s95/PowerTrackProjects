#ifndef STRING_STACK_H
#define STRING_STACK_H

#include <string>

#define MAX_HISTORY 100

using namespace std;

class StringStack
{
private:
    string arr[MAX_HISTORY];
    int top;

public:
    StringStack();

    bool isEmpty();
    bool isFull();

    void push(string page);
    void pop();
    string popValue();
    string peek();

    void display();
};

#endif