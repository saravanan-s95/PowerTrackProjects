#ifndef STACK_H
#define STACK_H

#include "Node.h"

class stack
{
    private:
        Node *top;    //it should ne private because no user should modify it
        int count ;
    public:
        stack();
        ~stack();
        void push(int);
        void pop();
        bool isempty();
        void display();
        int peek();
        int size();
        bool search(int);
        void reverse();
};

#endif
