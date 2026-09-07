#include "rbt.h"

int search(tree_t *root, data_t item)
{
    tree_t *current = root;

    while (current != NULL)
    {
        if (item == current->data)
        {
            return SUCCESS;
        }
        else if (item < current->data)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return FAILURE;
}