#include "apc.h"
#include <stdio.h>

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR,Dlist **tailR)
{
	/* Definition goes here */
	Dlist *num2 = *tail2;

    Dlist *sum_head = NULL;
    Dlist *sum_tail = NULL;

    Dlist *temp_head = NULL;
    Dlist *temp_tail = NULL;

    Dlist *add_head = NULL;
    Dlist *add_tail = NULL;

    int zeros = 0;

    while (num2)
    {
        temp_head = NULL;
        temp_tail = NULL;

        int carry = 0;
        int product;

        /* Add required trailing zeros */
        for (int i = 0; i < zeros; i++)
        {
            insert_at_first(&temp_head, &temp_tail, 0);
        }

        Dlist *num1 = *tail1;

        while (num1)
        {
            product = (num1->data * num2->data) + carry;

            carry = product / 10;
            product = product % 10;

            if (zeros == 0)
            {
                insert_at_first(&sum_head, &sum_tail, product);
            }
            else
            {
                insert_at_first(&temp_head, &temp_tail, product);
            }

            num1 = num1->prev;
        }

        if (carry)
        {
            if (zeros == 0)
                insert_at_first(&sum_head, &sum_tail, carry);
            else
                insert_at_first(&temp_head, &temp_tail, carry);
        }

        if (zeros != 0)
        {
            addition(&sum_head, &sum_tail,
                        &temp_head, &temp_tail,
                        &add_head, &add_tail);

            delete_list(&sum_head, &sum_tail);

            copy_list(&add_head, &add_tail,
                      &sum_head, &sum_tail);

            delete_list(&add_head, &add_tail);
            delete_list(&temp_head, &temp_tail);
        }

        num2 = num2->prev;
        zeros++;
    }

    while (sum_head && sum_head->next && sum_head->data == 0)
    {
        delete_first(&sum_head);
    }

    copy_list(&sum_head, &sum_tail, headR, tailR);

    delete_list(&sum_head, &sum_tail);
    delete_list(&temp_head, &temp_tail);
    delete_list(&add_head, &add_tail);

    return SUCCESS;
}
