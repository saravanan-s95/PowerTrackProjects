#include "header.h"
#include<stdio.h>

int main()
{
	start();
	return 0;
}


int start(void)
{
	tree_t *root = NULL;
	data_t data;
	data_t min;
	data_t max;
	int operation;
	char insert_more;

	while (1)
	{
		printf("\n================ MENU ================\n");
		printf("1. Insert element into BST\n");
		printf("2. Search for an element\n");
		printf("3. Delete specific element\n");
		printf("4. Print tree\n");
		printf("5. Find minimum value\n");
		printf("6. Find maximum value\n");
		printf("7. Delete minimum element\n");
		printf("8. Delete maximum element\n");
		printf("9. Exit\n");
		printf("======================================\n");
		printf("Enter your choice: ");

		if (scanf("%d", &operation) != 1)
		{
			printf("Invalid input\n");
			return FAILURE;
		}
 
		switch (operation)
		{
			case 1:
			{
				do
				{
					printf("Enter the data to insert: ");
					scanf("%d", &data);
					if (insert(&root, data) == SUCCESS)
						printf("INFO: Data inserted successfully\n");
					else
						printf("INFO: Duplicate data or memory allocation failed\n");

					printf("root-->");
					print_tree(root);
					printf("NULL");
					
					printf("\nWant to insert another element? [yY/nN]: ");
					scanf(" %c", &insert_more);

					while (insert_more != 'y' && insert_more != 'Y' &&
						   insert_more != 'n' && insert_more != 'N')
					{
						printf("Invalid option. Please enter y or n: ");
						scanf(" %c", &insert_more);
					}
				} while (insert_more == 'y' || insert_more == 'Y');
				break;
			}
			case 2: 
			{
				printf("Enter the data to search: ");
				scanf("%d", &data);
				if (search(root, data) == SUCCESS)
					printf("INFO: Data found\n");
				else
					printf("INFO: Data not found\n");
				break;
			}
			case 3:
			{
				printf("Enter the data to delete: ");
				scanf("%d", &data);
				if (delete(&root, data) == SUCCESS)
					printf("INFO: Element deleted successfully\n");
				else
					printf("INFO: Data not found or tree is empty\n");
				break;
			}
			case 4:
			{
				if(root==NULL)
					printf("Tree is empty\n");
			    else
				{
					printf("root-->");
					print_tree(root);
					printf("NULL");
				}
				break;
			}
			case 5:
			{
				if (find_minimum(&root, &min) == SUCCESS)
					printf("INFO: Minimum value is %d\n", min);
				else
					printf("INFO: Tree is empty\n");
				break;
			}
			case 6:
			{
				if (find_maximum(&root, &max) == SUCCESS)
					printf("INFO: Maximum value is %d\n", max);
				else
					printf("INFO: Tree is empty\n");
				break;
			}
			case 7:
			{
				if (delete_minimum(&root) == SUCCESS)
					printf("INFO: Minimum element deleted successfully\n");
				else
					printf("INFO: Tree is empty\n");
				break;
			}	
			case 8:
			{
				if (delete_maximum(&root) == SUCCESS)
					printf("INFO: Maximum element deleted successfully\n");
				else
					printf("INFO: Tree is empty\n");
				break;
			}
			case 9:
				return SUCCESS;
			default:
				printf("Invalid option. Choose 1 to 10.\n");
				break;				
		}
	}

}