/**************************************************************************************************************************************************************
*Title		: main function(Driver function)
*Description	: This function is used as the driver function for the all the functions
***************************************************************************************************************************************************************/
#include "rbt.h"

int display_min = 0;
int display_max = 0;

int main()
{
	/* Declare the tree stuctures */
	tree_t *root = NULL;
	data_t data;
	data_t minimum;
	data_t maximum;

	char option;

	while(1)
	{
		/* Display the menu */
		printf("1. Create a tree\n2. Display\n3. Search\n4. Delete\n5. Find Minimum\n6. Delete Minimum\n7. Find Maximum\n8. Delete Maximum\n9. Exit\n");

		/* Read the option for performing the operation */
		int operation;
		scanf("%d",&operation);

		/* Jump to the option entered by the user */
		do
		{
			switch (operation)
			{
				case 1:
				printf("Enter the data to be inserted into the RB Tree: ");
				scanf("%d", &data);
				if(insert(&root, data) == FAILURE)
				{
					break;
				}
				/* Modify the above line to handle the error */
				printf("\nNow Tree is balance\n");
				print_tree(root);
				break;

				case 2:
					print_tree(root);
					break;

				case 3:
					printf("Enter the data to search in the RB Tree: ");
					scanf("%d", &data);

					search_data(root,data);
					break;

				case 4:
					printf("Enter the data to be deleted from the RB Tree: ");
					scanf("%d", &data);
					if(delete(&root, data) == FAILURE)
					{
						break;
					}
					/* Modify the above line to handle the error */
					print_tree(root);
					break;

				case 5:
					display_min = 1;
					if(find_minimum(&root, &minimum) == FAILURE)
					{
						break;
					}
					/* Modify the above line to handle the error */
					print_tree(root);
					break;

				case 6:
					if(delete_minimum(&root) == FAILURE)
					{
						break;
					}
					/* Modify the above line to handle the error */
					print_tree(root);
					break;

				case 7:
					display_max = 1;
					if(find_maximum(&root,&maximum) == FAILURE)
					{
						break;
					}
					/* Modify the above line to handle the error */
					print_tree(root);
					break;	

				case 8:
					if(delete_maximum(&root) == FAILURE)
					{
						break;
					}
					/* Modify the above line to handle the error */
					print_tree(root);
					break;

				case 9:
					free_tree(root);
					return 0;
				
				default:
					printf("Invalid option\n");
					break;
			}
			printf("\nWant to continue? Press [yY | nN]: ");
			scanf("\n%c", &option);
		}while (option == 'y' || option == 'Y');
	}

	return 0;
}