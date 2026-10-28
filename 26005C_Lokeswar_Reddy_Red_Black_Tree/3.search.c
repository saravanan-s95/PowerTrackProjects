#include "header.h"

int search(tree_t *root, data_t data)
{
    while(root!=NULL)
    {
        if(data == root->data)
            return SUCCESS;
        else if(data < root->data)
            root=root->left;
        else
            root=root->right;
    }
    return FAILURE;
}