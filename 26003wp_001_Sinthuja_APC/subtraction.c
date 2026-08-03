#include "apc.h"

int subtraction(Dlist *head1, Dlist *tail1,
                Dlist *head2, Dlist *tail2,
                Dlist **res_head,
                Dlist **res_tail)
{
    int borrow = 0;
    int diff;
    int cmp;
   

    cmp = compare_lists(head1, head2);

    if(cmp == 0)
    {
        insert_first(res_head,
                     res_tail,
                     0);
        return 0;
    }
if(cmp < 0)
{
    Dlist *tmp;

    tmp = head1;
    head1 = head2;
    head2 = tmp;

    tmp = tail1;
    tail1 = tail2;
    tail2 = tmp;
}
    

    while(tail1)
    {
        int d1 = tail1->data;
        int d2 = 0;

        if(tail2)
            d2 = tail2->data;

        d1 = d1 - borrow;

        if(d1 < d2)
        {
            d1 += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        diff = d1 - d2;

        insert_first(res_head,
                     res_tail,
                     diff);

        tail1 = tail1->prev;

        if(tail2)
            tail2 = tail2->prev;
    }

    remove_leading_zeros(res_head,
                         res_tail);

    

    return 0;
}