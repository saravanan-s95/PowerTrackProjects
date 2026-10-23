#include "apc.h"

int division(Dlist **head1, Dlist **tail1,
             Dlist **head2, Dlist **tail2,
             Dlist **headR, Dlist **tailR)
{
    int count = 0;

	if(*head2 == NULL && (*head2)->data == 0)
	{
		printf("Dividing by Zero not Possible \n");
		return FAILURE;
	}

    while (list_compare(*head1, *tail1,*head2, *tail2))
    {
        /* Clear previous result */
        *headR = NULL;
        *tailR = NULL;

        /* Perform subtraction */
        subtraction(head1, tail1,
                    head2, tail2,
                    headR, tailR);

        count++;
        /*
         * Delete old dividend
         */
        delete_list(head1, tail1);

        /*
         * Transfer subtraction result
         * as the new dividend
         */
        *head1 = *headR;
        *tail1 = *tailR;

        /*
         * Detach result pointers.
         * The nodes now belong to head1/tail1.
         */
        *headR = NULL;
        *tailR = NULL;
    }

    printf("Ans is : %d\n", count);

    return SUCCESS;
}