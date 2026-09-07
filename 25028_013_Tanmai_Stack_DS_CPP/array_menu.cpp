#include <iostream>
#include "../include/stack.h"
#include "../include/array_menu.h"

using namespace std;

void arrayStackMenu()
{
    Stack s;
    int choice, value;

    do
    {
        cout << "\n========== ARRAY STACK ==========";
        cout << "\n1. Push";
        cout << "\n2. Pop";
        cout << "\n3. Peek";
        cout << "\n4. Display";
        cout << "\n5. Back";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                cout << "Enter value: ";
                cin >> value;
                s.push(value);
                break;

            case 2:
                s.pop();
                break;

            case 3:
                if(!s.isEmpty())
                    cout << "Top Element: " << s.peek() << endl;
                else
                    cout << "Stack is Empty\n";
                break;

            case 4:
                s.display();
                break;

            case 5:
                break;

            default:
                cout << "Invalid Choice\n";
        }

    } while(choice != 5);
}