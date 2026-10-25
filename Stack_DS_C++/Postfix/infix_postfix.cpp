#include "main.h"

int Stack :: Infix_Postfix_conversion(string &Infix_exp, string &Postfix_exp)
{
	/* Convert infix to postfix */
	for(int i=0;i<Infix_exp.length();i++)
	{
		/* Get char */
        char ch = Infix_exp[i];

		/* Check whether digit */
	    if(ch >= '0' && ch <= '9')
	    {
			/* Handle multiple digits */
            while(i<Infix_exp.length() && Infix_exp[i] >= '0' && Infix_exp[i] <= '9')
            {
                Postfix_exp += Infix_exp[i];
                i++;
            }
            Postfix_exp += ' ';
            i--; 
	    }

	    else if(ch == '(')
	    {
	        push(ch);
	    }
	    else if(ch == ')')
	    {
	        while(top != -1 && peek() != '(')
	        {
	            Postfix_exp += peek();
                Postfix_exp += ' ';
	            pop();
	        }
	        pop();
	    }
	    else
	    {
	        while(top != -1 && priority(ch) <= priority(peek()))
	        {
	            Postfix_exp += peek();
                Postfix_exp += ' ';
	            pop();
	        }
	        push(ch);
	    }
	}

	/* Peek and store in postfix array and pop till stack is empty */
	while(top != -1)
	{
	    Postfix_exp += peek();
        Postfix_exp += ' ';
	    pop();
	}
	return 0;
}