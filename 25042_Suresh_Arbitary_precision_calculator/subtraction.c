#include "apc.h"
#include <stdio.h>
int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR,Dlist **tailR)
{
	/* Definition goes here */
    Dlist *copy_head = NULL ;
    Dlist *copy_tail = NULL;
    copy_list(head1, tail1, &copy_head, &copy_tail);
    Dlist *temp1 = copy_tail;
    Dlist *temp2 = *tail2;
    int borrow = 0;
    int dif;

    while(temp1 || temp2)
    {
        if(temp1 && temp2 == NULL)
        {
            if(temp1 -> data < 0)
            {
                borrow = 1;
                if(temp1 -> prev != NULL)
                {
                    temp1 -> data += 10;
                    temp1 -> prev -> data -= 1;
                }
            }
            dif = temp1 -> data;
            insert_at_first(headR, tailR, dif);
            temp1 = temp1 -> prev;
        }
        if(temp1 && temp2)
        {
            if(temp1 -> data < temp2 -> data || temp1 -> data < 0)
            {
                if(temp1 -> prev != NULL)
                {
                    borrow = 1;
                    temp1 -> prev -> data -= 1;
                    temp1 -> data += 10;
                }
            }
            else
            {
                borrow = 0;
            }
            dif = temp1 -> data - temp2 -> data;
            insert_at_first(headR, tailR, dif);
            temp1 = temp1 -> prev;
            temp2 = temp2 -> prev;
        }
    }
    while(*headR && (*headR) -> next && (*headR) -> data == 0)
    {
        delete_first(headR);
    }
	
}
