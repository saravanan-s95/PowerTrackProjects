#include "rbt.h"

int delete_maximum(tree_t **root)
{
    data_t max;

    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    if (find_maximum(root, &max) == FAILURE)
    {
        return FAILURE;
    }

    return delete(root, max);
}