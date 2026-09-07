#include <iostream>
#include <string>
#include <cctype>
#include <limits>

#include "../include/stack.h"
#include "../include/postfix_evaluation.h"

using namespace std;

void postfixEvaluationMenu()
{
    Stack s;
    string postfix;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter Postfix Expression: ";
    getline(cin, postfix);

    for (char ch : postfix)
    {
        if (ch == ' ')
            continue;

        // Operand
        if (isdigit(ch))
        {
            s.push(ch - '0');
        }

        // Operator
        else
        {
            int operand2 = s.popValue();
            int operand1 = s.popValue();

            int result = 0;

            switch (ch)
            {
                case '+':
                    result = operand1 + operand2;
                    break;

                case '-':
                    result = operand1 - operand2;
                    break;

                case '*':
                    result = operand1 * operand2;
                    break;

                case '/':
                    if (operand2 == 0)
                    {
                        cout << "\nDivision by Zero Error!\n";
                        return;
                    }
                    result = operand1 / operand2;
                    break;

                default:
                    cout << "\nInvalid Operator!\n";
                    return;
            }

            s.push(result);
        }
    }

    cout << "\nResult = " << s.peek() << endl;

    cout << "\nPress Enter to return...";
    cin.get();
}