#include "rbt.h"

tree_t *search(tree_t *root, data_t key)
{
    while(root)
    {
        if(key == root->data)
            return root;

        if(key < root->data)
            root = root->left;
        else
            root = root->right;
    }

    return NULL;
}