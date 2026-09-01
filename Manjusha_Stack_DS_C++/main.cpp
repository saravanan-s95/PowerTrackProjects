/*
Name : Manjusha Thiruveedhula
Registration Number : 25048_005
Description : I implemented a Stack Data Structure in C++ using a linked-list-based approach. 
              The stack follows the LIFO principle. I implemented basic operations such as 
              push, pop, peek and display, and also added additional operations like search, reverse and size. 
              Push inserts an element at the top, pop removes the top element, 
              peek returns the top element without removing it, search finds a particular element, 
              reverse changes the order of the stack, display shows all elements, and size 
              returns the number of elements currently present.

*/
#include <iostream>
#include <cstdlib>
#include "stack.h"

using namespace std;

int main()
{
    stack s;
    int choice;

    while(1)
    {
        cout << "\n----------------- STACK ----------------" << endl;
        cout << "1. Push" << endl;
        cout << "2. Pop" << endl;
        cout << "3. Peek" << endl;
        cout << "4. Display" << endl;
        cout << "5. Size" << endl;
        cout << "6. Search" << endl;
        cout << "7. Reverse" << endl;
        cout << "8. Exit" << endl;
        cout << "----------------------------------------" << endl;

        cout << "Enter the choice : ";
        cin >> choice;

        switch(choice)
        {
            case 1:
            {
                int i;

                cout << "Enter number you want to push into stack: ";
                cin >> i;

                s.push(i);

                cout << "Element pushed successfully." << endl;
                break;
            }

            case 2:
            {
                s.pop();
                break;
            }

            case 3:
            {
                if(s.isempty())
                {
                    cout << "Stack is empty." << endl;
                }
                else
                {
                    cout << "The top most element is : "
                         << s.peek() << endl;
                }

                break;
            }

            case 4:
            {
                if(s.isempty())
                {
                    cout << "Stack is empty." << endl;
                }
                else
                {
                    cout << "The elements in the stack are : ";
                    s.display();
                }

                break;
            }

            case 5:
            {
                cout << "Size of stack : "
                     << s.size() << endl;

                break;
            }

            case 6:
            {
                int value;

                cout << "Enter element to search: ";
                cin >> value;

                if(s.search(value))
                {
                    cout << "Element found in the stack." << endl;
                }
                else
                {
                    cout << "Element not found." << endl;
                }

                break;
            }

            case 7:
            {
                if(s.isempty())
                {
                    cout << "Stack is empty." << endl;
                }
                else
                {
                    s.reverse();

                    cout << "Stack reversed successfully." << endl;
                    cout << "Stack after reverse: ";
                    s.display();
                }

                break;
            }

            case 8:
            {
                cout << "Thank you....." << endl;
                exit(0);
            }

            default:
            {
                cout << "Invalid choice." << endl;
                break;
            }
        }
    }

    return 0;
}