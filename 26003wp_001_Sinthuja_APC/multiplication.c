#include "apc.h"

int multiplication(Dlist *head1, Dlist *tail1,
                   Dlist *head2, Dlist *tail2,
                   Dlist **res_head,
                   Dlist **res_tail)
{
    int shift = 0;

    while(tail2)
    {
        Dlist *temp_head = NULL;
        Dlist *temp_tail = NULL;

        int carry = 0;

        Dlist *t1 = tail1;

        while(t1)
        {
            int prod = (t1->data * tail2->data) + carry;

            carry = prod / 10;

            insert_first(&temp_head,
                         &temp_tail,
                         prod % 10);

            t1 = t1->prev;
        }

        if(carry)
        {
            insert_first(&temp_head,
                         &temp_tail,
                         carry);
        }

        append_zeros(&temp_head,
                     &temp_tail,
                     shift);

        if(*res_head == NULL)
        {
            copy_list(temp_head,
                      res_head,
                      res_tail);
        }
        else
        {
            Dlist *new_head = NULL;
            Dlist *new_tail = NULL;

            addition(*res_head,
                     *res_tail,
                     temp_head,
                     temp_tail,
                     &new_head,
                     &new_tail);

            *res_head = new_head;
            *res_tail = new_tail;
        }

        shift++;

        tail2 = tail2->prev;
    }

    return 0;
}