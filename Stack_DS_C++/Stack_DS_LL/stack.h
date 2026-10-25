#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

#define SUCCESS 0
#define FAILURE -1

using data_t = int;

class Stack
{
    class Node
    {
        friend class Stack;

        data_t data;
        Node* link;

        Node(data_t data,Node* link) : data(data),link(link)
        {

        }
    };
    Node* top = nullptr;

    public:
        int Push(data_t data);
        int Pop();
        int Peek();
        int Peep();

        ~Stack()
        {
            Node* temp = top;
            while(temp != nullptr)
            {
                top = temp->link;
                delete(temp);
                temp = top;
            }
        }
};

#endif