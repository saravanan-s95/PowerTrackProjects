#include "main.h"

int Stack :: Infix_Prefix_conversion(string &Infix_exp, string &Prefix_exp)
{
    /* Convert from infix to prefix */
	for(int i=0;i<Infix_exp.length();i++)
	{
        /* Get char */
        char ch = Infix_exp[i];

        /* Check whether digit */
	    if(ch >= '0' && ch<= '9')
	    {
            /* Handle multiple digits */
            while(i<Infix_exp.length() && Infix_exp[i] >= '0' && Infix_exp[i] <= '9')
            {
                Prefix_exp += Infix_exp[i];
                i++;
            }
	        Prefix_exp += ' ';
            i--;
	    }

	    else if(ch == ')')
	    {
	        push(ch);
	    }

	    else if(ch == '(')
	    {
	        while(top != -1 && peek() != ')')
	        {
	            Prefix_exp += peek();
                Prefix_exp += ' ';
	            pop();
	        }
	        pop();
	    }

	    else
	    {
	        while(top != -1 && priority(ch) < priority(peek()))
	        {
	            Prefix_exp += peek();
                Prefix_exp += ' ';
	            pop();
	        }
	        push(ch);
	    }
	}

    /* Peek and store in Prefix array and pop till stack is empty */
	while(top != -1)
	{
	    Prefix_exp += peek();
        Prefix_exp += ' ';
	    pop();
	}
	return 0;
}