#include <iostream>
#include <string>
#include <limits>

#include "../include/string_stack.h"
#include "../include/browser_history.h"

using namespace std;

void browserHistoryMenu()
{
    StringStack backStack;
    StringStack forwardStack;

    string currentPage = "Home";

    int choice;
    string page;

    do
    {
        cout << "\n================================";
        cout << "\n      Browser History";
        cout << "\n================================";
        cout << "\nCurrent Page : " << currentPage;
        cout << "\n--------------------------------";
        cout << "\n1. Visit New Page";
        cout << "\n2. Back";
        cout << "\n3. Forward";
        cout << "\n4. Show Current Page";
        cout << "\n5. Show Back History";
        cout << "\n6. Show Forward History";
        cout << "\n7. Return to Main Menu";
        cout << "\n================================";
        cout << "\nEnter your choice : ";

        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice)
        {
            case 1:

                cout << "Enter Website : ";
                getline(cin, page);

                backStack.push(currentPage);

                currentPage = page;

                while(!forwardStack.isEmpty())
                    forwardStack.pop();

                break;

            case 2:

                if(backStack.isEmpty())
                {
                    cout << "\nNo Previous Page.\n";
                }
                else
                {
                    forwardStack.push(currentPage);
                    currentPage = backStack.popValue();
                }

                break;

            case 3:

                if(forwardStack.isEmpty())
                {
                    cout << "\nNo Forward Page.\n";
                }
                else
                {
                    backStack.push(currentPage);
                    currentPage = forwardStack.popValue();
                }

                break;

            case 4:

                cout << "\nCurrent Page : " << currentPage << endl;

                break;

            case 5:

                cout << "\nBack History\n";
                backStack.display();

                break;

            case 6:

                cout << "\nForward History\n";
                forwardStack.display();

                break;

            case 7:

                cout << "\nReturning to Main Menu...\n";
                break;

            default:

                cout << "\nInvalid Choice!\n";
        }

    }while(choice != 7);
}