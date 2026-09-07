#include <iostream>
#include <limits>

#include "../include/array_menu.h"
#include "../include/linked_menu.h"
#include "../include/parenthesis.h"
#include "../include/infix_postfix.h"
#include "../include/postfix_evaluation.h"
#include "../include/browser_history.h"
using namespace std;

int main()
{
    int choice;

    do
    {
        cout << "\n========================================";
        cout << "\n      STACK APPLICATIONS IN C++";
        cout << "\n========================================";
        cout << "\n1. Array Stack";
        cout << "\n2. Linked List Stack";
        cout << "\n3. Parentheses Matching";
        cout << "\n4. Infix to Postfix";
        cout << "\n5. Postfix Evaluation";
        cout << "\n6. Browser History";
        cout << "\n7. Exit";
        cout << "\n========================================";
        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\nPlease enter a valid number!\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                arrayStackMenu();
                break;

            case 2:
                linkedStackMenu();
                break;

            case 3:
                parenthesisMenu();
                break;

            case 4:
                infixToPostfixMenu();
                break;

            case 5:
                postfixEvaluationMenu();
                break;

            case 6:
                browserHistoryMenu();
                break;

            case 7:
                cout << "\nThank you for using Stack Applications!\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while (choice != 7);

    return 0;
}