#include "apc.h"
#include <stdio.h>

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR,Dlist **tailR)
{
	/* Definition goes here */
	 int quotient = 0;
    Dlist *product_head = NULL;
    Dlist *product_tail = NULL;
    Dlist *compare_head = NULL;
    Dlist *compare_tail = NULL;
    do
    {
        quotient++;
        Dlist *q_head = NULL;
        Dlist *q_tail = NULL;
        int copy = quotient;
        while(copy)
        {
            int digit = copy % 10;
            insert_at_first(&q_head, &q_tail, digit);
            copy /= 10;
        }
        delete_list(headR, tailR);
        multiplication(head2, tail2, &q_head, &q_tail, &product_head, &product_tail);
        delete_list(&compare_head, &compare_tail);
        compare_head = NULL;
        compare_tail = NULL;
        subtraction(head1, tail1, &product_head, &product_tail, &compare_head, &compare_tail);
        if(compare_head && compare_head -> data < 0)
        {
            --quotient;
        }
        delete_list(&q_head, &q_tail);
        delete_list(&product_head, &product_tail);
    }while(compare_head && compare_head -> data > 0);

    return quotient;
}
