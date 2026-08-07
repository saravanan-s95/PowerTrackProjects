
#include "apc.h"
int list_compare(Dlist **head1,Dlist **head2){
	int list1_count= 0;
	int list2_count =0;
	Dlist *t = *head1;
	while(t != NULL){
		list1_count++;
		t = t->next;
	}
	t = *head2;
	while(t != NULL){
		list2_count++;
		t = t->next;
	}
	if(list1_count > list2_count){
		return 1;
	}
	else if(list1_count < list2_count){
		return 0;
	}else if(list1_count == list2_count){
		Dlist *temp1 = *head1;
		Dlist *temp2 = *head2;
		while(temp1 != NULL)
		{
          if(temp1->data > temp2->data) return 1;
		  else if(temp1->data < temp2->data) return 0;
		  else{
			temp1 = temp1->next;
			temp2 = temp2->next;
		  }
		}
	}
	return 2;
}
void subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr,int flag)
{

	/* Definition goes here */
		Dlist *temp1 = *tail1;
	    Dlist *temp2 = *tail2;
		int data =0;
		int borrow = 0;
		while(temp1 || temp2)
		{
           if(temp1 != NULL && temp2 != NULL)
		   {
			   data = temp1->data - temp2->data - borrow;
               if(data < 0){
                   data +=10;
				 //  insert_at_begining(headr,tailr,(data - temp2->data));
				   borrow = 1;
			   }else{
				  borrow = 0;
                 // insert_at_begining(headr,tailr,data);
			   }
               insert_at_begining(headr,tailr,data);
			   temp1 = temp1->prev;
			   temp2 = temp2->prev;
		   }
		   else if(temp1!= NULL && temp2 == NULL)
		   {
              data = temp1->data - borrow;
			  if(data < 0 ){
				data = data +10;
				borrow = 1;
			  }else{
				borrow = 0;
			  }
			  insert_at_begining(headr,tailr,data);
			  temp1= temp1->prev;
		   }
		   
		}
		while (*headr != NULL && (*headr)->data == 0 && (*headr)->next != NULL) {
          Dlist *temp = *headr;
          *headr = (*headr)->next;
          (*headr)->prev = NULL;
          free(temp);
        }
		if(flag == 1) (*headr)->data = (*headr)->data*(-1);
}
