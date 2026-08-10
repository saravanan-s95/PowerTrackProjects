#include "rbt.h"

int delete_minimum(tree_t **root)
{
    tree_t *min_node;

    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    min_node = minimum_node(*root);

    if (min_node == NULL)
    {
        return FAILURE;
    }

    return delete(root, min_node->data);
}