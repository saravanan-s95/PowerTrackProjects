/**************************************************************************************************************************************************************
*Title		: main function(Driver function)
*Description	: This function is used as the driver function for the all the functions
***************************************************************************************************************************************************************/
#include "rbt.h"
int status;

int main()
{
	/* Declare the pointers */
	REDBLACKTREE tree;
	tree.root=NULL;
	
	int data;
	int  minimum;
	int maximum;
	int operation;
	char option;

	do
	{
		/* Display the menu */
		printf("1. Insert\n2. Delete\n3. Find Minimum\n4. Delete Minimum\n5. Find Maximum\n6. Delete Maximum\n7. Inoder\n8. Postorder\n 9. Preorder\n");
		printf("Enter your choice:");
		scanf("%d",&operation);
		/* Read the option for performing the operation */

		/* Jump to the option entered by the user */
		switch (operation)
		{
			case 1:
				printf("Enter the data to be inserted into the RB Tree: ");
				scanf("%d", &data);
				insert(&tree, data);
				if(status==SUCCESS)
					printf("INFO : Data Inserted Successfully\n");
				else if( status ==DUPLICATE)
					printf("INFO : Duplicate Found\n");
				else
					printf("INFO : Insertion was Unsuccessfull\n");
				print_tree(&tree);
				break;
			case 2:
				printf("Enter the data to be deleted from the RB Tree: ");
				scanf("%d", &data);
				// delete(&tree, data);
				if(status==NOELEMENT)
					printf("Element not found\n");
				else
					printf("INFO : deleted successfully\n");
				/* Modify the above line to handle the error */
				print_tree(&tree);
				break;
			case 3:
				status = find_minimum(tree.root, &minimum);
				if(status==FAILURE){
					printf("INFO : Tree is empty\n");
					break;
				}
				/* Modify the above line to handle the error */
				printf("Minimum data: %d\n", minimum);
				print_tree(&tree);
				break;
			case 4:
				status=delete_minimum(&tree);
				if(status==FAILURE)
					printf("INFO : Tree is empty\n");
				else{	
					printf("INFO : Deletion was Successfull\n");
					print_tree(&tree);
				}
				break;
			case 5:
				status = find_maximum(tree.root,&maximum);
				/* Modify the above line to handle the error */
				if(status==FAILURE){
					printf("INFO : Tree is empty\n");
					break;
				}
				printf("Maximum data: %d\n", maximum);
				print_tree(&tree);
				break;				
			case 6:
				status=delete_maximum(&tree);
				if(status==FAILURE)
					printf("INFO : Tree is empty\n");
				else{	
					printf("INFO : Deletion was Successfull\n");
					print_tree(&tree);
				}
				break;	
			case 7:
				inorder(tree.root);
				printf("\n");
				break;
			case 8:
				postorder(tree.root);
				break;
				printf("\n");
			case 9:
				preorder(tree.root);
				break;	
				printf("\n");		
		}
		printf("Want to continue? Press [yY | nN]: ");
		scanf("\n%c", &option);
	}while (option == 'y' || option == 'Y');

	return 0;
}
