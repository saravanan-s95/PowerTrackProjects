/*******************************************************************************************************************************************************************
*Title			: Delete Minimum
*Description		: This function deletes the minimum data from the given Red Black tree.
*Prototype		: int delete_minimum(tree_t **root);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete_minimum(tree_t **root)
{
    data_t min;

    if (root == NULL || *root == NULL)
        return FAILURE;

    if (find_minimum(root, &min) == FAILURE)
        return FAILURE;

    return delete(root, min);
}
