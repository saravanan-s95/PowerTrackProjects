
#include "apc.h"

void delete_list(Dlist **head,Dlist **tail){
	Dlist *temp=*head;
	while(temp != NULL){
		Dlist *f = temp;
		temp = temp->next;
		free(f);
	}
	*head = NULL;
	*tail = NULL;
}
void division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr,int *count)
{
	/* Definition goes here */
	int ret = 1;
     while(ret == 1)
	 {
        ret = list_compare(head1,head2);
        if(ret)
		{
			//printf("hiii");
			
			subtraction(head1,tail1,head2,tail2,headr,tailr,0);
			(*count)++;
			delete_list(head1,tail1);
			*head1 = *headr;
			*tail1 = *tailr;
			*headr = NULL;
			*tailr = NULL;

			if ((*head1)->data == 0 && (*head1)->next == NULL) {
                break; 
			}
		}else if(ret == 2){		
				(*count)++;
				break;
		}else {
			break;
		}
	 }
}


