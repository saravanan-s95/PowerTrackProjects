#ifndef APC_H
#define APC_H


#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
	struct node *prev;
	int data;
	struct node *next;
}Dlist;

#define POSITIVE 0
#define NEGATIVE -1
#define LIST_EMPTY -2

/* Include the prototypes here */
int insert_at_last(Dlist **head, Dlist **tail, int digit);

/* Function to insert an element as first node of the DLL */
int insert_at_first(Dlist **head, Dlist **tail, int data);

/*store the operands into the list */
void digit_to_list(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *argv[]);

/* Function to print the DLL*/
int print_list(Dlist *head);

/* Function to delete first node of the DLL */
int delete_first(Dlist **head);

/* Function to count number of nodes in list */
int list_length(Dlist *head);

/* Function to swap operand1 and operand2 between list1 and list2 */
void swap_operand(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2);

/*Addition */
int addition(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

/*subtraction*/
int subtraction(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

/*Multiplication*/
int multiplication(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);


/*Division */
int division(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

int delete_list(Dlist **head, Dlist **tail);

void copy_list(Dlist **head, Dlist **tail, Dlist **copy_head, Dlist **copy_tail);

#endif
