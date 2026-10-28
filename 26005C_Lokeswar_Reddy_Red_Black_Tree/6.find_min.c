#include "header.h"

int find_minimum(tree_t **root, data_t *min)
{
	tree_t *pres;
	
	if(root ==  NULL || *root==NULL || min == NULL)
		return FAILURE;
    
	pres=*root;

	while(pres->left != NULL)
		pres=pres->left;

	*min=pres->data;

	 return SUCCESS;
}
