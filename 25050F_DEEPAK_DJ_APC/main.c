#include "apc.h"
int check(char *str){
	int i=0;
	while(str[i]!= '\0'){
		if(str[i]<= '0' || str[i]> '9') return 0;
		i++;
	}
	return 1;
}
void insert_at_begining(Dlist **head,Dlist **tail,int data)
{
    Dlist *new = malloc(sizeof(Dlist));
  new->prev = NULL;
  new->data = data;
  if(*head == NULL && *tail == NULL)
  {
      *head = new;
      new->next = NULL;
      *tail = new;
      return;
  }else
  {
      
     (*head)->prev = new;
     new->next = *head;
     *head = new;
     return;
  }
  return;
}
/* Include the prototypes here */
void insert_at_end(Dlist **head,Dlist **tail,int data){
    Dlist *new = malloc(sizeof(Dlist));
    new->data = data;
    new->next = NULL;
  if(*head == NULL && *tail == NULL)
  {
      *tail = new;
      new->prev = NULL;
      *head = new;
      return;
  }
  else
  {
    new->prev = *tail;
    (*tail)->next = new;
    *tail = new;
    return;
  }
  return;
}

void print_list(Dlist *head)
{
     	/* Cheking the list is empty or not */
	if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
	else
	{
	   // printf("Head -> ");
	    while (head)		
	    {
		    /* Printing the list */
		    printf("%d", head -> data);

		    /* Travering in forward direction */
		    head = head -> next;
		    //if (head)
		        //printf("> ");
	    }
    	printf("\n");
    }
}

int main(int argc,char *argv[])
{
	/*clare the pointers */
	Dlist *head1 = NULL, *tail1 = NULL, *head2 = NULL, *tail2 = NULL, *headr = NULL, *tailr = NULL;
	char option, operator;
		/* Code for reading the inputs */
		if(argc < 4)
		{
			printf("\t Not enough arguments\n");
			printf("\t ENTER : ./a.out operand operator operand\n");
			return 0;
		}
        // if(check(argv[1])==0){
		// 	printf("\t Enter the correct number at operand 1\n");
		// 	return 0;
		// }
		// if(check(argv[3])==0){
		// 	printf("\t Enter the correct number at operand 2\n");
		// 	return 0;
		// }
        int sign1 = 1;
		int sign2 = 1;
		char *str = argv[1];
		int i=0;
		if(str[i]=='-'){
			sign1 = -1;
			i++;
		}
        while(str[i]!='\0')
		{
			int data = str[i]-'0';
			insert_at_end(&head1,&tail1,data);
			i++;
		}
      //  print_list(head1);
		char *str2 = argv[3];
		int j=0;
		if(str2[j]=='-'){
			sign2 = -1;
			j++;
		}
        while(str2[j]!='\0')
		{
			int data = str2[j]-'0';
			insert_at_end(&head2,&tail2,data);
			j++;
		}
       // print_list(head2);
        operator = argv[2][0];
		/* Function for extracting the operator */

		switch (operator)
		{
			case '+':
				/* call the function to perform the addition operation */
				if(sign1 ==1 && sign2 == 1){
			    	addition(&head1, &tail1,&head2, &tail2,&headr,&tailr);
				    printf("\t The addition of the two numbers is : ");
				    print_list(headr);
				}else if(sign1 == -1 && sign2 == -1){
					addition(&head1, &tail1,&head2, &tail2,&headr,&tailr);
				    printf("\t The addition of the two numbers is : -");
				    print_list(headr);
				}else if(sign1 == 1 && sign2 == -1){
					int flag = 0;
				    if(list_compare(&head1,&head2) == 1){
				     	subtraction(&head1, &tail1,&head2, &tail2,&headr,&tailr,flag);
				     	//printf("\tThe subtracted value is : ");
						 printf("\t The addition of the two numbers is : ");
		                print_list(headr);
				   }else{
					    flag =1;
					    subtraction(&head2, &tail2,&head1, &tail1,&headr,&tailr,flag);
					 //   printf("\tThe subtracted value is : ");
					    printf("\t The addition of the two numbers is : ");
		                print_list(headr);
				   }
				}else if(sign1 == -1 && sign2 == 1){
                     int flag = 0;
				    if(list_compare(&head1,&head2) == 1){
				     	subtraction(&head1, &tail1,&head2, &tail2,&headr,&tailr,flag);
				     	//printf("\tThe subtracted value is : ");
						 printf("\t The addition of the two numbers is : ");
		                print_list(headr);
				   }else{
					    flag =1;
					    subtraction(&head2, &tail2,&head1, &tail1,&headr,&tailr,flag);
					 //   printf("\tThe subtracted value is : ");
					    printf("\t The addition of the two numbers is : ");
		                print_list(headr);
				   }
				}
				break;
			case '-':	
				/* call the function to perform the subtraction operation */
				if(!strcmp(argv[1],argv[3])){
					printf("\tThe subtracted value is : 0\n");
					break;
				}
				if(sign1 == 1 && sign2 == 1){
				    int flag = 0;
				    if(list_compare(&head1,&head2) == 1){
				    	subtraction(&head1, &tail1,&head2, &tail2,&headr,&tailr,flag);
					    printf("\tThe subtracted value is : ");
		                print_list(headr);
				    }else{
					    flag =1;
					    subtraction(&head2, &tail2,&head1, &tail1,&headr,&tailr,flag);
					    printf("\tThe subtracted value is : ");
		                print_list(headr);
				    }
				}else if(sign1 == -1 && sign2 == -1){
				    int flag = 0;
				    if(list_compare(&head1,&head2) == 1){
				    	subtraction(&head2, &tail2,&head1, &tail1,&headr,&tailr,flag);
					    printf("\tThe subtracted value is : ");
		                print_list(headr);
				    }else{
					    flag =1;
					    subtraction(&head1, &tail1,&head2, &tail2,&headr,&tailr,flag);
					    printf("\tThe subtracted value is : ");
		                print_list(headr);
				    }
				}else if(sign1 ==1 && sign2 == -1){
					addition(&head1, &tail1,&head2, &tail2,&headr,&tailr);
				 //   printf("\t The addition of the two numbers is : ");
				    printf("\tThe subtracted value is : ");
				    print_list(headr);
				}
				else if(sign1 == -1 && sign2 == 1)
				{
                   addition(&head1, &tail1,&head2, &tail2,&headr,&tailr);
				 //   printf("\t The addition of the two numbers is : ");
				    printf("\tThe subtracted value is : -");
				    print_list(headr);
				}
				break;
			case 'x':	
				/* call the function to perform the multiplication operation */
				multiplication(&head1, &tail1,&head2, &tail2,&headr,&tailr);
				printf("\t The multiplication of the two numbers is : ");
				int finalsign = sign1*sign2;
				if(finalsign ==-1 && headr->data != 0) printf("-");
				print_list(headr);
				break;
			case '/':	
				/* call the function to perform the division operation */
				int count =0;
				division(&head1, &tail1,&head2, &tail2,&headr,&tailr,&count);
				int fsign = sign1*sign2;
				if(fsign ==-1 && count !=0)
                   printf("The division is : -%d\n",count);
				else 
				    printf("The division is : %d\n",count);
				break;
			default:
				printf("\tInvalid Input operator:-( Try again...\n");
				break;
	    }
	return 0;
}
