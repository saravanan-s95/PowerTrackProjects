#include "main.h"

/* function definition to find the minimum node */
int find_minimum(tree_t **root, data_t *min)
{
    if (*root == NULL)
    {
        return FAILURE;
    }

    tree_t *temp = *root;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }

    *min = temp->data;
    return SUCCESS;
}