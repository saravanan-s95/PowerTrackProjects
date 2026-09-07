#include <iostream>
#include <string>
#include <limits>

#include "../include/char_stack.h"
#include "../include/parenthesis.h"

using namespace std;

bool isMatchingPair(char open, char close)
{
    return ((open == '(' && close == ')') ||
            (open == '{' && close == '}') ||
            (open == '[' && close == ']'));
}

void parenthesisMenu()
{
    CharStack s;
    string expression;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter an expression: ";
    getline(cin, expression);

    bool valid = true;

    for (char ch : expression)
    {
        if (ch == '(' || ch == '{' || ch == '[')
        {
            s.push(ch);
        }
        else if (ch == ')' || ch == '}' || ch == ']')
        {
            if (s.isEmpty())
            {
                valid = false;
                break;
            }

            char top = s.peek();
            s.pop();

            if (!isMatchingPair(top, ch))
            {
                valid = false;
                break;
            }
        }
    }

    if (!s.isEmpty())
        valid = false;

    if (valid)
        cout << "\nExpression is VALID.\n";
    else
        cout << "\nExpression is INVALID.\n";

    cout << "\nPress Enter to return to Main Menu...";
    cin.get();
}