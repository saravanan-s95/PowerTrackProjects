#ifndef MAIN_H
#define MAIN_H

#define SIZE_STACK 50

#include <iostream> 
#include <algorithm>
#include <string>
using namespace std;

class Stack
{
    int top = -1;
    int stack[SIZE_STACK];

    public:
        int Infix_Prefix_conversion(string &Prefix_exp, string &Infix_exp);
        int Prefix_Eval(string &Prefix_exp);
        int priority(char opr);
        void push(int data);
        int peek();
        void pop();
};

#endif