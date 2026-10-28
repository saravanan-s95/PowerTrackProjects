#include "header.h"

int delete_minimum(tree_t **root)
{
	tree_t *pres;

	if (root == NULL || *root == NULL)
		return FAILURE;

	pres = *root;
	
	while (pres->left != NULL)
		pres = pres->left;

	return delete(root, pres->data);
}
