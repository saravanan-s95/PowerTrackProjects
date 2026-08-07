/*******************************************************************************************************************************************************************
*Title			: Multiplication
*Description		: This function performs multiplication of two given large numbers and store the result in the resultant list.
*Prototype		: int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR);
*Input Parameters	: head1: Pointer to the first node of the first double linked list.
			: tail1: Pointer to the last node of the first double linked list.
			: head2: Pointer to the first node of the second double linked list.
			: tail2: Pointer to the last node of the second double linked list.
			: headR: Pointer to the first node of the resultant double linked list.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "apc.h"

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr, Dlist **tailr)
{
	/* Definition goes here */
	Dlist *headr1=NULL;
	Dlist *headr2=NULL;
	Dlist *tailr1=NULL;
	Dlist *tailr2=NULL;
	Dlist *temp1 = *tail1;
	Dlist *temp2 = *tail2;
	int carry=0;
	int cnt =0;
	while(temp2){
		headr2=NULL;
        tailr2=NULL;
		for(int i=0;i<cnt;i++)
		{
			insert_at_begining(&headr2,&tailr2,0);
		}
		carry = 0;
		while(temp1){
            int data = temp1->data * temp2->data + carry;
			carry = 0;
			if(cnt == 0){
               if(data > 9){
			   insert_at_begining(&headr1,&tailr1,(data%10));
			   carry = data/10;
		       }else{
			   insert_at_begining(&headr1,&tailr1,data);
		       }
		    }
		    else{
			 if(data > 9){
			   insert_at_begining(&headr2,&tailr2,(data%10));
			   carry = data/10;
		       }else{
			   insert_at_begining(&headr2,&tailr2,data);
		       }
		    }
			temp1 = temp1->prev;
		}
		if(carry != 0 && cnt ==0)
		{
		   insert_at_begining(&headr1,&tailr1,carry);
		   carry =0;
		}
		else if (carry != 0)
		{
            insert_at_begining(&headr2,&tailr2,carry);
		    carry =0;
		}
		if(cnt >= 1)
		{
			addition(&headr1,&tailr1,&headr2,&tailr2,headr,tailr);
			delete_list(&headr1,&tailr1);
			delete_list(&headr2,&tailr2);
			headr1 = *headr;
			tailr1 = *tailr;
            headr2=NULL;
            tailr2=NULL;
			*headr = NULL;
			*tailr = NULL;
		}
		cnt++;
		temp1 = *tail1;
		temp2 = temp2->prev;
	}
    *headr = headr1;
    *tailr = tailr1;
   // print_list(*headr);
	return SUCCESS;
}
