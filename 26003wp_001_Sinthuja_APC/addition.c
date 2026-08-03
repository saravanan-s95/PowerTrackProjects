#include "apc.h"

int addition(Dlist *head1,Dlist *tail1,
             Dlist *head2,Dlist *tail2,
             Dlist **res_head,Dlist **res_tail)
{
    int carry = 0;
    int sum;
    int digit1;
    int digit2;

    while(tail1 != NULL || tail2 != NULL)
    {
        digit1 = 0;
        digit2 = 0;

        if(tail1 != NULL)
        {
            digit1 = tail1->data;
            tail1 = tail1->prev;
        }

        if(tail2 != NULL)
        {
            digit2 = tail2->data;
            tail2 = tail2->prev;
        }

        sum = digit1 + digit2 + carry;

        carry = sum / 10;

        insert_first(res_head,
                     res_tail,
                     sum % 10);
    }

    if(carry)
    {
        insert_first(res_head,
                     res_tail,
                     carry);
    }

    return 0;
}