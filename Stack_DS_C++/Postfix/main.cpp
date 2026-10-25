#include "main.h"

int main()
{
	string Infix_exp, Postfix_exp;
    char ch;
	int choice, result;
	Stack stk;

	cout << "Enter the Infix expression : ";
	cin >> Infix_exp;

	stk.Infix_Postfix_conversion(Infix_exp, Postfix_exp);
	cout << "PostFix expression : " << Postfix_exp << endl;
	
	result = stk.Postfix_Eval(Postfix_exp);
	cout << "\nResult : " << result << endl;
	return 0;
}
