#include"apc.h"

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;

    int carry = 0;

    while (temp1 != NULL || temp2 != NULL)
    {
        int data = carry;

        if (temp1 != NULL)
        {
            data = data + temp1->data;
            temp1 = temp1->prev;
        }

        if (temp2 != NULL)
        {
            data = data + temp2->data;
            temp2 = temp2->prev;
        }

        insert_first(headR, tailR, data % 10);

        carry = data / 10;
    }

    if (carry != 0)
    {
        insert_first(headR, tailR, carry);
    }

    // printf("Ans Is : ");
    // print_Dlist(&(*headR), &(*tailR));
    // printf("\n");
	// free(*headR);
    return SUCCESS;
}
