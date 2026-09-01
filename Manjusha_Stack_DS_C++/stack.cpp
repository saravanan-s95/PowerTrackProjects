#include "Node.h"
#include "stack.h"
#include <iostream>
using namespace std;


stack::stack()
{
    top = NULL;
    count = 0;
}


bool stack::isempty()
{
    if(top == NULL)
    {
        return true;
    }
    return false;
}


void stack::push(int d)
{
    Node *newnode = new Node;

    if(newnode == NULL)
    {
        cout<<"Memory is not allocated"<<endl;
        return;
    }
    newnode->data = d;
    newnode->link = top;

    top = newnode;

    count++;

    cout<<"pushing into the stack is successful"<<endl;
}


void stack::pop()
{
    if(isempty() == true)
    {
        cout<<"Stack is underflow....POP wont works"<<endl;
        return ;
    }
    Node *temp = top;

    top = top->link;

    delete temp;

    count--;

    cout << "Pop is successfull" << endl;
}

int stack::peek()
{
    if (isempty())
    {
        cout << "Stack is Empty" << endl;
        return -0;
    }

    return top->data;
}


void stack::display()
{
    Node *temp = top;

    while(temp != NULL)
    {
        cout<<temp->data;
        cout<<"->";
        temp = temp -> link;
    }
    cout<<"NULL"<<endl;
}

int stack::size()
{
    return count;
}


bool stack::search(int value)
{
    Node *temp = top;

    while(temp != NULL)
    {
        if(temp->data == value)
        {
            return true;
        }

        temp = temp->link;
    }

    return false;
}


void stack::reverse()
{
    Node *prev = NULL;
    Node *current = top;
    Node *next = NULL;

    while(current != NULL)
    {
        next = current->link;
        current->link = prev;
        prev = current;
        current = next;
    }

    top = prev;
}


stack :: ~stack()
{

}