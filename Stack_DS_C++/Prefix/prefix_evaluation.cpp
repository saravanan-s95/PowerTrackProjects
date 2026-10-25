#include "main.h"

int Stack :: Prefix_Eval(string &Prefix_exp)
{
    /* Reset top to -1 */
    top = -1;

    /* Evaluate prefix expression */
    for(int i=Prefix_exp.length() - 1;i>=0;i--)
    {
        /* Get char */
        char ch = Prefix_exp[i];

        if(ch == ' ')
        {
            continue;
        }

        /* Check whether digit */
        if(ch >= '0' && ch <= '9')
        {
            /* Handle multiple digits */
            int num = 0;
            int place = 1;

            while(i >= 0 && Prefix_exp[i] >= '0' && Prefix_exp[i] <= '9')
            {
                num += (Prefix_exp[i] - '0')*place;
                place *= 10;
                i--;
            }
            /* Push the number */
            push(num);
            i++;
        }
        
        else
        {
            /* Peek and store the numbers and pop */
            int op1 = peek();
            pop();
            int op2 = peek();
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
    /* Return the final result */
    return peek();
}