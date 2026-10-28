#include "header.h"

int find_maximum(tree_t **root, data_t *max)
{
	tree_t *pres;

	if(root == NULL || *root==NULL || max==NULL)
		return FAILURE;

	pres = *root;
	
	while(pres->right != NULL)
		pres = pres->right;

	*max=pres->data;

	return SUCCESS;
}
