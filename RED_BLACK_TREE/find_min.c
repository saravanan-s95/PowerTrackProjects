#include "rbt.h"

int find_minimum(tree_t **root, data_t *min)
{
    /* Check whether the tree is empty */
    if (*root == NULL)
    {
        return LIST_EMPTY;
    }

    /* Start traversal from the root */
    tree_t *temp = *root;

    /* Traverse to the leftmost node */
    while (temp->left != NULL)
    {
        temp = temp->left;
    }

    /* Store the minimum value */
    *min = temp->data;

   
    return SUCCESS;
}