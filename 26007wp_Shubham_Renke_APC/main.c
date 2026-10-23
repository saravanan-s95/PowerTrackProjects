#include <stdio.h>
#include "supportFun.c"
#include "add.c"
#include "subtraction.c"
#include "multiplication.c"
#include "division.c"
#include "apc.h"

int main(int argc, char *argv[])
{
	/* Declare the pointers */
	for(int i = 1; i < 4; i++)
	{
		if(argv[i] == NULL)
		{
			printf("invalid Input !!! \n");
			return 0;
		}
	}

	Dlist *head1 = NULL, *tail1 = NULL;
	Dlist *head2 = NULL, *tail2 = NULL;
	Dlist *headR = NULL, *tailR = NULL;

	char operator = argv[2][0];
	digit_to_list(&head1, &tail1, &head2, &tail2, argv);

	switch (operator)
	{
	case '+':
		/* call the function to perform the addition operation */
		addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		printf("Ans is : ");
		print_Dlist(&headR, &tailR);
		printf("\n");
		break;
	case '-':
		int flag = 0;
		int ret = list_compare(head1, tail1, head2, tail2);
		if (ret)
		{
			subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		}
		else
		{
			flag = 0;
			subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
			flag = 1;
		}

		if (flag == 1)
		{
			headR->data = headR->data * -1;
		}
		printf("Ans Is : ");
		print_Dlist(&headR, &tailR);
		printf("\n");
		break;
	case 'x':
		/* call the function to perform the multiplication operation */
		multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		break;
	case '/':
		// 		/* call the function to perform the division operation */
		division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
		break;
	default:
		printf("Invalid Input:-( Try again...\n");
		break;
	}
	return 0;
}
