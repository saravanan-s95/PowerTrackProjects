/*******************************************************************************************************************************************************************
*Title			: Find Maximum
*Description		: This function finds the maximum data from the given Red Black tree.
*Prototype		: int find_maximum(tree_t **root, data_t *max);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: max – Maximum data present in the tree is collected via this pointer.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int find_maximum(tree_t **root, data_t *max)
{
	/* Definition goes here */

	/* Tree is empty */
	if(*root == NULL)
	{
		printf("Tree is empty\n");
		return FAILURE;
	}

	tree_t* temp = *root;

	/* Find the rightmost node */
	while(temp->right)
	{
		temp = temp->right;
	}
	/* Store in max */
	*max = temp->data;

	extern int display_max;
	if(display_max)
	{
		printf("Maximum node in the given Red Black Tree is : (%d)--(%s)\n",*max,temp->color == RED ? "RED->0" : "BLACK->1");
	}
	display_max = 0;
	return SUCCESS;
}
