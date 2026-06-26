#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "apc.h"
int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Invalid expression...\n");
		return 0;
	}

	/* Declare the pointers */

	Dlist *head1 = NULL, *tail1 = NULL;
	int digit;
	int n1_sign_char = 0;
	int n1_sign = POSITIVE;
	// check if argv[1] is empty
	if (argv[1] == NULL)
	{
		printf("No input provided\n");
		return 1;
	}
	if (argv[1][0] == '+')
	{
		n1_sign_char = 1;
	}
	if (argv[1][0] == '-')
	{
		n1_sign = NEGATIVE;
		n1_sign_char = 1;
	}
	for (int i = n1_sign_char; argv[1][i] != '\0'; i++)
	{
		if (isdigit(argv[1][i]))
		{
			digit = argv[1][i] - '0'; // convertting argv[1] from string to integer
			insert_at_last(&head1, &tail1, digit);
		}
		else
		{
			printf("Invalid 1st operand \n");
			return 0;
		}
	}
	while (head1 != NULL && head1->next != NULL) // deleting unwanted zeros at begining from DL
	{
		if (head1->data == 0)
			delete_first(&head1);
		else
			break;
	}
	printf("Operand 1 : ");
	if (n1_sign == NEGATIVE)
	{
		printf("-");
	}

	if (print_list(head1) == LIST_EMPTY)
	{
		printf("Input proper operands..\n");
		// help();
		return 0;
	}

	Dlist *head2 = NULL, *tail2 = NULL;
	int n2_sign = POSITIVE;
	int n2_sign_char = 0;
	if (argv[3] == NULL)
	{
		printf("No input Provided\n");
		return 1;
	}
	if (argv[3][0] == '+')
	{
		n2_sign_char = 1;
	}
	if (argv[3][0] == '-')
	{
		n2_sign = NEGATIVE;
		n2_sign_char = 1;
	}
	for (int i = n2_sign_char; argv[3][i] != '\0'; i++)
	{

		if (isdigit(argv[3][i]))
		{
			digit = argv[3][i] - '0';
			insert_at_last(&head2, &tail2, digit);
		}
		else
		{
			printf("Invalid 2nd operand \n");
			return 0;
		}
	}
	while (head2 && head2->next && head2->data == 0)
	{
		delete_first(&head2);
	}
	printf("Operand 2 : ");
	if (n2_sign == NEGATIVE)
	{
		printf("-");
	}

	if (print_list(head2) == LIST_EMPTY)
	{
		printf("Input proper operands..\n");
		return 0;
	}

	Dlist *headR = NULL, *tailR = NULL;

	char operator = argv[2][0];
	if (strlen(argv[2]) != 1)
	{
		printf("Invalid operator\n");
		return 0;
	}
	switch (operator)
	{
	case '+':
		printf("Operation :addition\n");
		printf("%s + %s =", argv[1], argv[3]);
		/*If both the operands either + or - we use this incase both - we use to indicate using seperate condition*/
		if (n1_sign == n2_sign)
		{
			addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);

			if (n1_sign == NEGATIVE)
			{
				printf("-");
			}
			print_list(headR);
			break;
		}
		/*From this block confirm that both operands have diffrent operands*/
		int swap = 0;
		if (list_length(head1) < list_length(head2))
		{
			swap_operand(&head1, &tail1, &head2, &tail2);
			swap = 1;
		}
		else if (list_length(head1) == list_length(head2))
		{
			for (int i = 0; i < strlen(argv[1]); i++)
			{
				if (argv[1][i] < argv[3][i])
				{
					swap_operand(&head1, &tail1, &head2, &tail2);
					swap = 1;
					break;
				}
			}
		}
		subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		if (swap == 0)
		{
			// head1 is bigger → use n1_sign
			if (n1_sign == NEGATIVE) // example -456 + 123 no swapping just subtraction
				printf("-");
		}
		else
		{
			// head2 is bigger → use n2_sign
			if (n2_sign == NEGATIVE) // example 123 + -456 swapping happens before +456 - 123 and swap=1 so if swapped then add - sign
				printf("-");
		}
		print_list(headR);
		break;
	case '-':
		/* call the function to perform the subtraction operation */
		printf("Operation : Subtraction\n");
		printf("%s - %s =", argv[1], argv[3]);
		if (strcmp(argv[1], argv[3]) == 0)
		{
			insert_at_first(&headR, &tailR, 0);
			print_list(headR);
			break;
		}
		swap = 0;
		if (list_length(head1) < list_length(head2))
		{
			swap_operand(&head1, &tail1, &head2, &tail2);
			swap = 1;
		}
		else if (list_length(head1) == (list_length(head2)))
		{
			for (int i = 0; i < strlen(argv[1]); i++)
			{
				if (argv[1][i] < argv[3][i])
				{
					swap_operand(&head1, &tail1, &head2, &tail2);
					swap = 1;
					break;
				}
			}
		}
		// Both operands have same sign
		if (n1_sign == n2_sign)
		{
			if (n1_sign == POSITIVE)
			{
				if (swap) // 127 - 856
				{
					printf("-");
				}
				subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR); // 856 - 123
				print_list(headR);
			}

			else
			{
				if (swap == 0) //(-856) - (-123)
				{
					printf("-");
				}
				subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR); //(-123) - (-856)
				print_list(headR);
			}
			break;
		}

		// operands have diffrent sign
		else
		{
			if (n1_sign == POSITIVE) // (+123) - (-856)
			{
				addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
				print_list(headR);
			}
			else
			{ //(-123) - (+856)
				printf("-");
				addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
				print_list(headR);
			}
		}

		break;
	case 'x':
	case 'X':
	case '*':
		printf("Operation : Multiplication\n");
		if (head1->data == 0 || head2->data == 0)
		{
			printf("%s x %s = 0\n", argv[1], argv[3]);
			break;
		}
		if (list_length(head1) < list_length(head2))
		{
			swap_operand(&head1, &tail1, &head2, &tail2);
		}
		multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		int product_sign;
		if (n1_sign == n2_sign)
		{
			product_sign = POSITIVE;
		}
		else
		{
			product_sign = NEGATIVE;
		}
		printf("%s x %s = %c", argv[1], argv[3], (product_sign ? '-' : '\0'));
		print_list(headR);
		break;

	case '/':
		printf("Operation : division\n");
		if (head2->data == 0)
		{
			printf("Invalid operand..⚠️ Divisor can not be Zero.\n\n");
			break;
		}
		if (list_length(head2) > list_length(head1))
		{
			printf("%s / %s = 0\n", argv[1], argv[3]);
			break;
		}
		else if (list_length(head2) == list_length(head1))
		{
			int flag = 0;
			int i = n1_sign_char, j = n2_sign_char;
			while (argv[1][i])
			{
				if (argv[1][i] > argv[3][j])
				{
					break;
				}
				if (argv[1][i] < argv[3][j])
				{
					printf("%s / %s = 0\n", argv[1], argv[3]);
					flag = 1;
					break;
				}
				i++;
				j++;
			}
			if (flag)
				break;
		}
		int quotient_sign;
		if (n1_sign == n2_sign)
		{
			quotient_sign = POSITIVE;
		}
		else
		{
			quotient_sign = NEGATIVE;
		}
		int res = division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		printf("%s / %s = %c%d\n", argv[1], argv[3], (quotient_sign ? '-' : '\0'), res);
		break;
	default:
		printf("Invalid Input:-( Try again...\n");
		break;
	}
	return 0;
}
/*Function to insert new node from tail at DLL*/
int insert_at_last(Dlist **head, Dlist **tail, int digit)
{
	Dlist *newnode = malloc(sizeof(Dlist));
	if (newnode == NULL)
	{
		return FAILURE;
	}
	newnode->data = digit;
	newnode->prev = NULL;
	newnode->next = NULL;
	if (*head == NULL && *tail == NULL)
	{
		*head = *tail = newnode;
		return SUCCESS;
	}
	else
	{
		newnode->prev = *tail;
		(*tail)->next = newnode;
		*tail = newnode;
		return SUCCESS;
	}
}
/*Function to delete first node of DLL*/
int delete_first(Dlist **head)
{
	if (*head == NULL)
		return FAILURE;
	Dlist *temp = *head;
	*head = (*head)->next;
	if (*head != NULL)
	{
		(*head)->prev = NULL;
	}
	free(temp);
	return SUCCESS;
}
/*Function to print the DLL*/
int print_list(Dlist *head)
{
	if (head == NULL)
	{
		printf("List is Empty\n");
		return LIST_EMPTY;
	}
	Dlist *temp = head;

	while (temp)
	{
		printf("%d", temp->data);
		temp = temp->next;
	}

	printf("\n");
	return SUCCESS;
}

/* Function to count number of nodes in list */
int list_length(Dlist *head)
{
	int count = 0;
	if (head == NULL)
	{
		return count;
	}
	while (head)
	{
		count++;
		head = head->next;
	}
	return count;
}
/*Function to swap operand1 and operand2 betweeen list1 and list 2*/
void swap_operand(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2)
{
	// For head swap
	Dlist *temp;
	temp = *head1;
	*head1 = *head2;
	*head2 = temp;

	// For tail swap
	temp = *tail1;
	*tail1 = *tail2;
	*tail2 = temp;
}
/*Function to insert an element as first node*/
int insert_at_first(Dlist **head, Dlist **tail, int data)
{
	Dlist *newnode = malloc(sizeof(Dlist));
	if (newnode == NULL)
	{
		return FAILURE;
	}
	newnode->data = data;
	newnode->prev = NULL;
	newnode->next = NULL;

	if (*head == NULL && *tail == NULL)
	{
		*head = newnode;
		*tail = newnode;
		return SUCCESS;
	}
	else
	{
		newnode->next = *head;
		(*head)->prev = newnode;
		*head = newnode;
		return SUCCESS;
	}
}
void copy_list(Dlist **head, Dlist **tail, Dlist **copy_head, Dlist **copy_tail)
{
    if(*head == NULL)
    {
        return;
    }
    delete_list(copy_head, copy_tail);

    Dlist *temp = *head;
    while(temp)
    {
        insert_at_last(copy_head, copy_tail, temp -> data);
        temp = temp -> next;
    }
}

int delete_list(Dlist **head, Dlist **tail)
{
    if(*head == NULL && *tail == NULL)
    return FAILURE;

    Dlist *temp = *head;
    Dlist *prev = NULL;
    while(temp != NULL)
    {
        prev = temp;
        temp = temp -> next;
        free(prev);
    }
    *head = NULL;
    *tail = NULL;
    return SUCCESS;
}