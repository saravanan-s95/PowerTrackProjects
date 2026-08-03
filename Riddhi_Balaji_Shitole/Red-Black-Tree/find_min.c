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
    if (*root == NULL)
        return FAILURE;

    tree_t *temp = *root;

    while (temp != NULL && temp->left != NULL)
        temp = temp->left;

    *min = temp->data;

    return SUCCESS;
}