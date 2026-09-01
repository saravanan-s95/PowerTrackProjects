#include "rbt.h"

tree_t *search(tree_t *root, data_t item)
{
    /* Traverse the tree until the node is found or tree ends */
    while (root != NULL)
    {
        /* Check if the current node contains the required data */
        if (item == root->data)
        {
            /* Return the address of the found node */
            return root;
        }

        /* Move to the left subtree if the item is smaller */
        else if (item < root->data)
        {
            root = root->left;
        }

        /* Otherwise move to the right subtree */
        else
        {
            root = root->right;
        }
    }

    /* Data not found in the tree */
    return NULL;
}