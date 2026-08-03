#include "apc.h"

int division(Dlist *head1, Dlist *tail1,
             Dlist *head2, Dlist *tail2,
             Dlist **res_head,
             Dlist **res_tail)
{
    long long dividend;
    long long divisor;
    long long count = 0;

    dividend = list_to_number(head1);
    divisor  = list_to_number(head2);

    if(divisor == 0)
    {
        printf("ERROR : Divide By Zero\n");
        return -1;
    }

    if(dividend == 0)
    {
        insert_last(res_head,
                    res_tail,
                    0);
        return 0;
    }

    if(dividend < divisor)
    {
        insert_last(res_head,
                    res_tail,
                    0);
        return 0;
    }

    while(dividend >= divisor)
    {
        dividend = dividend - divisor;
        count++;
    }

    number_to_list(count,
                   res_head,
                   res_tail);

    return 0;
}