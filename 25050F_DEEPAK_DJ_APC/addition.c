
#include "apc.h"

void addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr)
{
	/* Definition goes here */
	Dlist *temp1 = *tail1;
	Dlist *temp2 = *tail2;
    int carry =0;
	int data=0;
	while(temp1 || temp2 ){
		if(temp1 != NULL && temp2 != NULL){
		  data = temp1->data + temp2->data + carry;
		  carry = 0;
		  if(data > 9){
			insert_at_begining(headr,tailr,(data%10));
			carry = data/10;
		  }else{
			insert_at_begining(headr,tailr,data);
		  }
		  temp1 = temp1->prev;
		  temp2 = temp2->prev;
		}else if (temp1 != NULL) {
           data = temp1->data + carry;
           carry = 0;
         if (data > 9) {
           insert_at_begining(headr, tailr, (data % 10));
             carry = data / 10;
        }else {
        insert_at_begining(headr, tailr, data);
         }
        temp1 = temp1->prev;
         }else if (temp2 != NULL) {
            data = temp2->data + carry;
          carry = 0;
          if(data > 9){
            insert_at_begining(headr, tailr, (data % 10));
             carry = data / 10;
          }else{
                insert_at_begining(headr, tailr, data);
           }
         temp2 = temp2->prev;
        }
	}
	if(carry != 0){
		insert_at_begining(headr,tailr,carry);
		carry =0;
	}
	
  //  print_list(*headr);
}

