#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include "../include/char_stack.h"
#include "../include/infix_postfix.h"

using namespace std;

// Check whether a character is an operator
bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' ||
            ch == '*' || ch == '/' ||
            ch == '^');
}

// Return precedence of operators
int precedence(char op)
{
    switch(op)
    {
        case '^':
            return 3;

        case '*':
        case '/':
            return 2;

        case '+':
        case '-':
            return 1;

        default:
            return 0;
    }
}

void infixToPostfixMenu()
{
    CharStack s;

    string infix;
    string postfix = "";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter Infix Expression: ";
    getline(cin, infix);

    for(char ch : infix)
    {
        // Ignore spaces
        if(ch == ' ')
            continue;

        // Operand
        if(isalnum(ch))
        {
            postfix += ch;
        }

        // Opening bracket
        else if(ch == '(')
        {
            s.push(ch);
        }

        // Closing bracket
        else if(ch == ')')
        {
            while(!s.isEmpty() && s.peek() != '(')
            {
                postfix += s.peek();
                s.pop();
            }

            if(!s.isEmpty())
                s.pop(); // Remove '('
        }

        // Operator
        else if(isOperator(ch))
        {
            while(!s.isEmpty() &&
                  precedence(s.peek()) >= precedence(ch))
            {
                postfix += s.peek();
                s.pop();
            }

            s.push(ch);
        }
    }

    // Pop remaining operators
    while(!s.isEmpty())
    {
        postfix += s.peek();
        s.pop();
    }

    cout << "\nPostfix Expression : " << postfix << endl;

    cout << "\nPress Enter to return...";
    cin.get();
}