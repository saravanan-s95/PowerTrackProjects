/*******************************************************************************************************************************************************************
*Title			: Delete Maximum
*Description		: This function deletes the maximum data from the given Red Black tree.
*Prototype		: int delete_maximum(tree_t **root);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete_maximum(REDBLACKTREE* tree)
{
	int max;
	int status=find_maximum(tree->root,&max);
	if(status==FAILURE)
		return FAILURE;
	else{
		delete(tree,max);
	}
	return SUCCESS;
}
