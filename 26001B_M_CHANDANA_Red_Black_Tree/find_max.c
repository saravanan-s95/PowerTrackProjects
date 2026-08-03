#include "main.h"

/* function definition to find the minimum node */
int find_maximum(tree_t **root, data_t *max)
{
    if (*root == NULL)
    {
        return FAILURE;
    }

    tree_t *temp = *root;
    while (temp->right != NULL)
    {
        temp = temp->right;
    }

    *max = temp->data;
    return SUCCESS;
}