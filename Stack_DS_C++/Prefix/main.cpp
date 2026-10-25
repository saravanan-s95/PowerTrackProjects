#include "main.h"

int main()
{
	string Infix_exp, Prefix_exp;
    char ch;
	int choice, result;
	Stack stk;

	cout << "Enter the Infix expression : ";
	cin >> Infix_exp;

	reverse(Infix_exp.begin(),Infix_exp.end());
	stk.Infix_Prefix_conversion(Infix_exp, Prefix_exp);
	reverse(Prefix_exp.begin(),Prefix_exp.end());
	cout << "PreFix expression : " << Prefix_exp << endl;

	//reverse(Prefix_exp.begin(),Prefix_exp.end());
	result = stk.Prefix_Eval(Prefix_exp);
	cout << "\nResult : " << result << endl;
	return 0;
}
