#include "types.h"
#include "lexical.h"

Status parantheses_handler(Stack *stk, char ch, char *f_name)
{
    if (ch == '(')
        stk->arr[++(stk->top)] = ')';

    else if (ch == '{')
        stk->arr[++(stk->top)] = '}';

    else if (ch == '[')
        stk->arr[++(stk->top)] = ']';

    else
    {
        // If stack is empty
        if (stk->top == -1)
        {
            printf("ERROR: Unexpected '%c'\nIn file %s\n", ch, f_name);
            return e_failure;
        }

        // If the top doesn't match
        else if (stk->arr[stk->top] != ch)
        {
            printf("ERROR: Expected '%c' but found '%c'\nIn File: %s\n", stk->arr[stk->top], ch, f_name);
            return e_failure;
        }

        stk->top--;
    }

    return e_success;
}