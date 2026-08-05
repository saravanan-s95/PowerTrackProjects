#include "rbt.h"

int delete_minimum(REDBLACKTREE* tree)
{
	int min;
	int status=find_minimum(tree->root,&min);
	if(status==FAILURE)
		return FAILURE;
	else{
		delete(tree,min);
	}
	return SUCCESS;
}
