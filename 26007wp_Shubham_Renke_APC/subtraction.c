#include "apc.h"


int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	/* Definition goes here */
	Dlist *temp1 = *tail1;
	Dlist *temp2 = *tail2;

	int data;
	int borrow = 0;

	while(temp1 != NULL)
	{
		int digit1 = temp1->data - borrow;
		int digit2 = 0;

		if(temp2 != NULL)
		{
			digit2 = temp2->data;
		}

		if( digit1 < digit2)
		{
			digit1 = digit1 + 10;
			borrow = 1;
		}
		else{
			borrow = 0;
		}

		data = digit1 - digit2;
		insert_first(headR,tailR,data);
		temp1 = temp1->prev;
		if(temp2!= NULL)
		{
			temp2=temp2->prev;
		}
	}
	remove_leading_zeros(headR,tailR);
	return SUCCESS;
}
