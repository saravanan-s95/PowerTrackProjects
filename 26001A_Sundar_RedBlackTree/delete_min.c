/*******************************************************************************************************************************************************************
*Title			: Delete Minimum
*Description		: This function deletes the minimum data from the given Red Black tree.
*Prototype		: int delete_minimum(tree_t **root);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete_minimum(tree_t **root)
{
	/* Definition goes here */

	/* Find minimum value in the tree */
	int min;
	if(find_minimum(root,&min) == FAILURE)
	{
		return FAILURE;
	}

	tree_t* min_node = *root;
	tree_t* parent = NULL;

	/* Find minimum node and its parent */
	while(min != min_node->data)
	{
		if(min < min_node->data)
		{
			parent = min_node;
			min_node = min_node->left;
		}
	}

	/* Minimum node is root */
	if(min_node == *root)
	{
		if(min_node->right)
		{
			*root = min_node->right;
			(*root)->color = BLACK;
		}
		else
		{
			*root = NULL;
		}	
		free(min_node);
		return SUCCESS;
	}

	/* Minimum node is red */
	if(min_node->color == RED)
	{
		/* Make left of parent null */
		parent->left = NULL;
		/* Delete node and return success */
		free(min_node);
	}
	
	/* Minimum node is black and there is no node to its right */
	else if(min_node->color == BLACK && min_node->right == NULL)
	{
		/* Make left of parent null */
		parent->left = NULL;
		/* Delete minimum node */
		free(min_node);
		/* Handle double black cases */
		delete_fix(root,parent,parent->right);
	}

	/* Minimum node is black and node exists to its right */
	else if(min_node->color == BLACK && min_node->right)
	{
		/* Copy minimum node's right value into minimum node */
		min_node->data = min_node->right->data;

		/* Delete right of minimum node */
		free(min_node->right);
		/* Make right of minimum node null */
		min_node->right = NULL;
	}
	return SUCCESS;
}