#include "apc.h"

int multiplication(Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR, Dlist **tailR)
{
    Dlist *temp1;
    Dlist *temp2;

    Dlist *headR1 = NULL;
    Dlist *tailR1 = NULL;

    Dlist *headR2 = NULL;
    Dlist *tailR2 = NULL;

    int carry;
    int data;
    int count = 0;

    temp2 = *tail2;

    while (temp2 != NULL)
    {
        /* Add zeros according to position */
        for (int i = 0; i < count; i++)
        {
            insert_first(&headR2, &tailR2, 0);
        }

        temp1 = *tail1;
        carry = 0;

        /* Multiply one digit of number2 with entire number1 */
        while (temp1 != NULL)
        {
            data = (temp1->data * temp2->data) + carry;

            insert_first(&headR2, &tailR2, data % 10);

            carry = data / 10;

            temp1 = temp1->prev;
        }

        /* Add remaining carry */
        if (carry != 0)
        {
            insert_first(&headR2, &tailR2, carry);
        }

        /*
         * First partial product
         */
        if (count == 0)
        {
            headR1 = headR2;
            tailR1 = tailR2;

            headR2 = NULL;
            tailR2 = NULL;
        }
        else
        {
            /*
             * Add previous result + current partial product
             */
            addition(&headR1, &tailR1,
                     &headR2, &tailR2,
                     headR, tailR);

            /* Delete old partial lists */
			
            delete_list(&headR1, &tailR1);
            delete_list(&headR2, &tailR2);

            /* Result becomes new partial result */
            headR1 = *headR;
            tailR1 = *tailR;

            *headR = NULL;
            *tailR = NULL;
        }

        count++;
        temp2 = temp2->prev;
    }

    /* Copy final result to headR/tailR */

	printf("Ans Is : ");
    print_Dlist(&headR1, &tailR2);
    printf("\n");
    return SUCCESS;
}
