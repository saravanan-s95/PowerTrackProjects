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
    data_t max;

    if (root == NULL || *root == NULL)
        return FAILURE;

    if (find_maximum(root, &max) == FAILURE)
        return FAILURE;

    return delete(root, max);
}