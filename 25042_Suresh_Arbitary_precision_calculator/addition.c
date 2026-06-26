#include <stdio.h>
#include <stdlib.h>
#include "apc.h"

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	/* Definition goes here */
	*headR = NULL;
	*tailR = NULL;
	Dlist *temp1 = *tail1;
	Dlist *temp2 = *tail2;
	int carry = 0, digit = 0;
	while (temp1 || temp2)
	{
		if (temp1 && temp2)
		{
			digit = temp1->data + temp2->data + carry;
													   
			if (digit > 9)
			{
				carry = digit / 10;
				digit = digit % 10;
			}
			else
			{
				carry = 0;
			}
			temp1 = temp1->prev;
			temp2 = temp2->prev;
		}
		else if (temp2 == NULL)
		{
			digit = temp1->data + carry;
			if (digit > 9)
			{
				carry = digit / 10;
				digit = digit % 10;
			}
			else
			{
				carry = 0;
			}
			temp1 = temp1->prev;
		}
		else if (temp1 == NULL)
		{
			digit = temp2->data + carry;
			if (digit > 9)
			{
				carry = digit / 10;
				digit = digit % 10;
			}
			else
			{
				carry = 0;
			}
			temp2 = temp2->prev;
		}
		insert_at_first(headR,tailR,digit);
	}
    if(carry){
		insert_at_first(headR,tailR,carry);
	}
   while(*headR && (*headR) -> next && (*headR) -> data == 0)
    {
        delete_first(headR);
    }

}
