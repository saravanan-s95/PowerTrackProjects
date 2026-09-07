#include "rbt.h"

int find_minimum(tree_t **root, data_t *min)
{
    tree_t *temp;

    if (root == NULL || *root == NULL || min == NULL)
    {
        return FAILURE;
    }

    temp = *root;

    while (temp->left != NULL)
    {
        temp = temp->left;
    }

    *min = temp->data;

    return SUCCESS;
}