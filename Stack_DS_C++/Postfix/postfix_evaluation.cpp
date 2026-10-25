
#include "main.h"

int Stack :: Postfix_Eval(string &Postfix_exp)
{
    /* Reset top to -1 */
    top = -1;

    /* Evaluate postfix expression */
    for(int i=0;i<Postfix_exp.length();i++)
    {
        /* Get char */
        char ch = Postfix_exp[i];

        if(ch == ' ')
        {
            continue;
        }

        /* Check whether digit */
        if(ch >= '0' && ch <= '9')
        {
            /* Handle multiple digits */
            int num = 0;
            while(i<Postfix_exp.length() && Postfix_exp[i] >= '0' && Postfix_exp[i] <= '9')
            {
                num = num * 10 + (Postfix_exp[i] - '0');
                i++;
            }
            push(num);
        }
        
        else
        {
            /* Peek and store the numbers and pop */
            int op2 = peek();
            pop();
            int op1 = peek();
            pop();
            
            int res;
            switch(ch)
            {
                case '+':
                res = op1 + op2;
                break;
                
                case '-':
                res = op1 - op2;
                break;
                
                case '*':
                res = op1 * op2;
                break;
                
                case '/':
                res = op1 / op2;
                break;
                
                default:
                return 0;
            }
            /* Push result */
            push(res);
        }
    }
    /* Return final value */
    return peek();
}