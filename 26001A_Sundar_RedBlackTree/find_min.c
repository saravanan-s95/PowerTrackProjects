/*******************************************************************************************************************************************************************
*Title			: Find Minimum
*Description		: This function finds the minimum data from the given Red Black tree.
*Prototype		: int find_minimum(tree_t **root, data_t *min);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: min – Minimum data present in the tree is collected via this pointer.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int find_minimum(tree_t **root, data_t *min)
{
	/* Definition goes here */

	/* Tree is empty */
	if(*root == NULL)
	{
		printf("Tree is empty\n");
		return FAILURE;
	}

	tree_t* temp = *root;

	/* Find the leftmost node */
	while(temp->left)
	{
		temp = temp->left;
	}
	/* Store in min */
	*min = temp->data;

	extern int display_min;
	if(display_min)
	{
		printf("Minimum node in the given Red Black Tree is : (%d)--(%s)\n",*min,temp->color == RED ? "RED->0" : "BLACK->1");
	}
	display_min = 0;
	return SUCCESS;
}
