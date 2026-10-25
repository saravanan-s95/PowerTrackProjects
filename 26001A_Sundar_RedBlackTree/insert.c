/*******************************************************************************************************************************************************************
*Title			: Insertion
*Description		: This function performs inserting the new data into the given Red Black tree.
*Prototype		: int insert(tree_t **root, data_t item);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – New data to be inserted into the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int insert(tree_t **root, data_t item)
{
	/* Definition goes here */

	/* Allocate new node memory */
	tree_t* new = malloc(sizeof(tree_t));
	if(new == NULL)
	{
		printf("Memory allocation failed\n");
		return FAILURE;
	}

	/* Update node data */
	new->left = NULL;
	new->right = NULL;
	new->color = RED;
	new->data = item;

	if(*root == NULL)
	{
		/* Assign root to new node and color it to black and return */
		new->color = BLACK;
		*root = new;
		return SUCCESS;
	}

	tree_t* parent = *root;

	/* Find parent node */
	while(1)
	{
		if(item == parent->data)
		{
			printf("Element duplication not allowed\n");
			free(new);
			return FAILURE;
		}

		if(item < parent->data)
		{
			if(parent->left == NULL)
			{
				break;
			}

			parent = parent->left;
		}
		else
		{
			if(parent->right == NULL)
			{
				break;
			}

			parent = parent->right;
		}
	}

	/* Insert data based on binary search */
	if(item < parent->data)
	{
		parent->left = new;
	}
	else if(item > parent->data)
	{
		parent->right = new;
	}

	printf("Before Balancing Red Black Tree is : \n");
	print_tree(*root);

	/* Parent color is black */
	if(parent->color == BLACK)
	{
		return SUCCESS;
	}

	/* Else fix red-red violation */
	fix_violation(root,new);
	(*root)->color = BLACK;	
	return SUCCESS;
}