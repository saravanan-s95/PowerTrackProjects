#include "header.h"

int delete_maximum(tree_t **root)
{
	tree_t *pres;

	if (root == NULL || *root == NULL)
		return FAILURE;

	pres = *root;
	
	while (pres->right != NULL)
		pres = pres->right;

	return delete(root, pres->data);
}
