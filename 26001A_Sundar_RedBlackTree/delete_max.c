/*******************************************************************************************************************************************************************
*Title			: Delete Maximum
*Description		: This function deletes the maximum data from the given Red Black tree.
*Prototype		: int delete_maximum(tree_t **root);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete_maximum(tree_t **root)
{
	/* Definition goes here */

	/* Find maximum value in the tree */
	int max;
	if(find_maximum(root,&max) == FAILURE)
	{
		return FAILURE;
	}

	tree_t* max_node = *root;
	tree_t* parent = NULL;

	/* Find maximum node and its parent */
	while(max != max_node->data)
	{
		if(max > max_node->data)
		{
			parent = max_node;
			max_node = max_node->right;
		}
	}

	/* Maximum node is root */
	if(max_node == *root)
	{
		if(max_node->left)
		{
			*root = max_node->left;
			(*root)->color = BLACK;
		}
		else
		{
			*root = NULL;
		}	
		free(max_node);
		return SUCCESS;
	}

	/* Maximum node is red */
	if(max_node->color == RED)
	{
		/* Make right of parent null */
		parent->right = NULL;
		/* Delete node and return success */
		free(max_node);
	}
	
	/* Maximum node is black and there is no node to its left */
	else if(max_node->color == BLACK && max_node->left == NULL)
	{
		/* Make right of parent null */
		parent->right = NULL;
		/* Delete maximum node */
		free(max_node);
		/* Handle double black cases */
		delete_fix(root,parent,parent->left);
	}

	/* Maximum node is black and node exists to its left */
	else if(max_node->color == BLACK && max_node->left)
	{
		/* Copy maximum node's left value into maximum node */
		max_node->data = max_node->left->data;

		/* Delete left of maximum node */
		free(max_node->left);
		/* Make left of maximum node null */
		max_node->left = NULL;
	}
	return SUCCESS;
}