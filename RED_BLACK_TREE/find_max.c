#include "rbt.h"

int find_maximum(tree_t **root, data_t *max)
{
    /* Check whether the tree is empty */
    if (*root == NULL)
    {
        return LIST_EMPTY;
    }

    /* Start traversal from the root */
    tree_t *temp = *root;

    /* Traverse to the rightmost node */
    while (temp->right != NULL)
    {
        temp = temp->right;
    }

    /* Store the maximum value */
    *max = temp->data;

    
    return SUCCESS;
}