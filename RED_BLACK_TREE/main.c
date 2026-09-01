/*
	NAME               : K.NIRMALA JYOTHI
	REG NO             : 25048_037
	SUBMISSION DATE    : 


	DESCRIPTION:

	1. main()

		Purpose: 

			Acts as the driver function of the Red‑Black Tree application.

		Process:

			Displays a menu of operations.

			Accepts user input for insertion, display, search, deletion, finding minimum/maximum nodes, and deleting minimum/maximum nodes.

			Calls the appropriate functions to perform each operation.

			Continues execution until the user chooses to exit.

		Return: 

			Returns 0 when the program terminates successfully.

	2. create_a_tree(tree_t **root, data_t item)

		Purpose: 
			
			Inserts a new node into the Red‑Black Tree.

		Process:

			Allocates memory for a new node and initializes it as RED.

			Traverses the tree to find the correct insertion position.

			Rejects duplicate values.

			Inserts the node and calls fix_insert() to restore Red‑Black properties.

			Displays the tree before and after balancing.

		Return: 

			Returns SUCCESS, DUPLICATE, or FAILURE.

	3. fix_insert(tree_t **root, tree_t *node)

		Purpose: 
		
			Restores Red‑Black Tree properties after insertion.

		Process:

			Handles violations caused by two consecutive RED nodes.

			Performs recoloring when the uncle node is RED.

			Performs rotations (LL, LR, RR, RL cases) when the uncle node is BLACK.

			Ensures the root node remains BLACK.

		Return: 
		
			No explicit return; modifies the tree in place.

	4. left_rotate(tree_t **root, tree_t *x)

		Purpose: 
		
			Performs a left rotation around the given node.

		Process:

			Promotes the right child as the new parent.

			Adjusts child and parent pointers.

			Used during balancing operations.

		Return: 
		
			No explicit return; modifies the tree in place.

	5. right_rotate(tree_t **root, tree_t *y)

		Purpose: 
			
			Performs a right rotation around the given node.

		Process:

			Promotes the left child as the new parent.

			Updates parent and child links.

			Used during balancing operations.

		Return: 
		
			No explicit return; modifies the tree in place.

	6. print_tree(tree_t *root)

		Purpose: 
		
			Displays the tree using inorder traversal.

		Process:

			Traverses the left subtree, prints the current node, then traverses the right subtree.

			Prints both the node value and its color (RED or BLACK).

		Return: 
		
			No explicit return; prints output to console.

	7. search(tree_t *root, data_t item)

		Purpose: 
		
			Searches for a specified element in the Red‑Black Tree.

		Process:

			Traverses left or right based on BST rules.

			Returns the address of the node if found.

		Return: 
		
			Returns pointer to the node if found, otherwise NULL.

	8. successor(tree_t *node)

		Purpose: 
		
			Finds the inorder successor of a given node.

		Process:

			If the node has a right subtree, returns the leftmost node of that subtree.

			Otherwise, moves upward using parent pointers until it finds the successor.

		Return: 
		
			Returns pointer to the successor node, or NULL if none exists.

	9. delete(tree_t **root, data_t item)

		Purpose: 
		
			Deletes the specified node from the Red‑Black Tree.

		Process:

			Searches for the node to delete.

			Handles four cases:

			Leaf node.

			Node with only left child.

			Node with only right child.

			Node with two children (using inorder successor).

			Updates parent‑child relationships.

			Prints the successor (next node) with its color.

			Frees the deleted node.

			Calls fix_delete() if a BLACK node was removed.

		Return: 
		
			Returns SUCCESS, DATA_NOT_FOUND, or LIST_EMPTY.

	10. find_maximum(tree_t **root, data_t *max)

		Purpose: 
		
			Finds the largest element in the tree.

		Process:

			Traverses continuously to the rightmost node.

			Stores the maximum value in the given variable.

		Return: 
		
			Returns SUCCESS if found, otherwise LIST_EMPTY.

	11. find_minimum(tree_t **root, data_t *min)

		Purpose: 
		
			Finds the smallest element in the tree.

		Process:

			Traverses continuously to the leftmost node.

			Stores the minimum value in the given variable.

		Return: 
		
			Returns SUCCESS if found, otherwise LIST_EMPTY.

	12. delete_maximum(tree_t **root)

		Purpose: 
		
			Deletes the maximum (rightmost) node in the tree.

		Process:

			Finds the maximum node and its parent.

			Reconnects the parent’s right pointer to the maximum node’s left child.

			Updates parent pointers accordingly.

			Frees the deleted node.

			Calls fix_delete() if a BLACK node was removed.

		Return: 
		
			Returns SUCCESS or LIST_EMPTY.

	13. delete_minimum(tree_t **root)

		Purpose: 
		
			Deletes the minimum (leftmost) node in the tree.

		Process:

			Finds the minimum node and its parent.

			Updates the parent’s left pointer to the minimum node’s right child.

			Updates parent pointers accordingly.

			Frees the deleted node.

			Calls fix_delete() if a BLACK node was removed.

		Return: 
		
			Returns SUCCESS or LIST_EMPTY.

	14. fix_delete(tree_t **root, tree_t *x)

		Purpose: 
		
			Restores Red‑Black Tree properties after deletion when a BLACK node is removed.

		Process:

			Handles the “double black” problem by repeatedly checking the sibling of x.

			Cases handled:

			Sibling is RED → recolor and rotate.

			Sibling is BLACK with BLACK children → recolor sibling RED and move up.

			Sibling is BLACK with at least one RED child → perform rotations and recoloring.

			Ensures the root is always BLACK at the end.

		Return: 
		
			No explicit return; modifies the tree in place.

*/

#include "rbt.h"

int main()
{
	/* Declare the root pointer of the Red-Black Tree */
	tree_t *root = NULL;

	/* Variables to store user input and minimum/maximum values */
	data_t data;
	data_t minimum;
	data_t maximum;

	/* Variable to continue or exit the program */
	char option;

	do
	{
		/* Variable to store menu choice */
		int operation;

		/* Variable to store the return status of functions */
		int status;

		/* Display the menu */
		printf(BLUE "\n--------------------RED BLACK TREE OPERATIONS------------------\n" RESET);
		printf(BLUE "\n1. Create a tree\n" RESET);
		printf(BLUE "2. Display\n" RESET);
		printf(BLUE "3. Search a node\n" RESET);
		printf(BLUE "4. Find Maximum node in tree\n" RESET);
		printf(BLUE "5. Find Minimum node in tree\n" RESET);
		printf(BLUE "6. Deletion\n" RESET);
		printf(BLUE "7. Delete Maximum node\n" RESET);
		printf(BLUE "8. Delete Minimum node\n" RESET);
		printf(BLUE "9. Exit\n\n" RESET);

		/* Read the option for performing the operation */
		printf(MAGENTA "Enter the choice:" RESET);
		scanf("%d",&operation);

		/* Jump to the option entered by the user */
		switch (operation)
		{
			/*---------------- CREATE TREE ----------------*/
			case 1:

				/* Read the data to be inserted */
				printf(YELLOW "\nEnter the data to be inserted into the RB Tree: " RESET);
				scanf("%d", &data);

				/* Insert the node into the Red-Black Tree */
				status = create_a_tree(&root, data);

				/* Check the insertion status */
				if (status == SUCCESS)
				{
					printf(GREEN "\n\nNode created successfully.\n" RESET);
				}
				else if (status == DUPLICATE)
				{
					printf(RED_COLOR "\nDuplicate data.\n" RESET);
				}
				else
				{
					printf(RED_COLOR "\nMemory allocation failed.\n" RESET);
				}
			break;
			
			/*---------------- DISPLAY TREE ----------------*/
			case 2:

				printf(YELLOW "\nDisplay\n" RESET);

				/* Check whether the tree exists */
				if (root != NULL)
				{
					/* Print the tree in inorder traversal */
					print_tree(root);
				}
				else
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;

			/*---------------- SEARCH NODE ----------------*/
			case 3:
			
				/* Pointer to store the searched node */
				tree_t *found;

				/* Read the element to search */
				printf(YELLOW "\nEnter the element which you want to search from the Red Black Tree: " RESET);
				scanf("%d", &data);

				/* Search the node */
				found = search(root, data);

				/* Check whether the node is found */
				if (found != NULL)
				{
					printf(GREEN "\nNode is found: (%d)--(%s)\n" RESET, found->data,(found->color == RED) ? "RED->0" : "BLACK->1");
				}
				else if (root == NULL)
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}
				else
				{
					printf(RED_COLOR "\nData not found.\n" RESET);
				}
			
			break;

			/*---------------- FIND MAXIMUM ----------------*/
			case 4:

				/* Find the maximum node */
				if (find_maximum(&root, &maximum) == SUCCESS)
				{
					tree_t *temp = root;

					/* Traverse to the rightmost node */
					while (temp->right != NULL)
					{
						temp = temp->right;
					}

					/* Display the maximum node with its color */
					printf(GREEN "\nMaximum node in the given Red Black Tree is : (%d)--(%s)\n" RESET,temp->data,(temp->color == RED) ? "RED->0" : "BLACK->1");
				}
				else
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;

			/*---------------- FIND MINIMUM ----------------*/
			case 5:

				/* Find the minimum node */
				if (find_minimum(&root, &minimum) == SUCCESS)
				{
					tree_t *temp = root;

					/* Traverse to the leftmost node */
					while (temp->left != NULL)
					{
						temp = temp->left;
					}

					/* Display the minimum node with its color */
					printf(GREEN "\nMinimum node in the given Red Black Tree is : (%d)--(%s)\n" RESET,temp->data,(temp->color == RED) ? "RED->0" : "BLACK->1");
				}
				else
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;

			/*---------------- DELETE NODE ----------------*/
			case 6:
			
				printf(YELLOW "\nEnter the data to be deleted from the RB Tree: " RESET);
				scanf("%d", &data);

				status = delete(&root, data);

				if (status == SUCCESS) 
				{
					printf(GREEN "\n%d is deleted successfully.\n" RESET, data);

					if (root != NULL) 
					{
						printf(GREEN "\nTree after deletion:\n\n" RESET);
						print_tree(root);
					} 
					else 
					{
						printf(RED_COLOR "\nTree is empty.\n" RESET);
					}
				} 

				else if (status == DATA_NOT_FOUND) 
				{
					printf(RED_COLOR "\nData not found.\n" RESET);
				} 

				else if (status == LIST_EMPTY) 
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;


			/*---------------- DELETE MAXIMUM ----------------*/
			case 7:

				if (find_maximum(&root, &maximum) == SUCCESS)
				{
					printf(GREEN "\nTree Before deleting the maximum node:\n" RESET);

					print_tree(root);

					printf(GREEN "\n\nTree After deleting the maximum node:\n" RESET);

					delete_maximum(&root);

					print_tree(root);
				}
				else
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;


			/*---------------- DELETE MINIMUM ----------------*/
			case 8:

				if (find_minimum(&root, &minimum) == SUCCESS)
				{
					printf(GREEN "\nTree Before deleting the minimum node:\n" RESET);
					
					print_tree(root);

					printf(GREEN "\n\nTree After deleting the minimum node:\n" RESET);

					delete_minimum(&root);
					
					print_tree(root);
				}
				else
				{
					printf(RED_COLOR "\nTree is empty.\n" RESET);
				}

			break;

			/*---------------- EXIT ----------------*/
			case 9:
				return SUCCESS;


			/*---------------- INVALID CHOICE ----------------*/
			default:

				printf(RED_COLOR "\nInvalid choice\n\n" RESET);

			break;	

		}

		/* Ask the user whether to continue */
		printf(CYAN  "\n\nWant to continue? Press [yY | nN]: " RESET);
		scanf(" %c", &option);

	}while (option == 'y' || option == 'Y');

	return SUCCESS;
}
