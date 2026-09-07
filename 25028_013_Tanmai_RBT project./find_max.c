#include "rbt.h"

int find_maximum(tree_t **root, data_t *max)
{
    tree_t *temp;

    if (root == NULL || *root == NULL || max == NULL)
    {
        return FAILURE;
    }

    temp = *root;

    while (temp->right != NULL)
    {
        temp = temp->right;
    }

    *max = temp->data;

    return SUCCESS;
}