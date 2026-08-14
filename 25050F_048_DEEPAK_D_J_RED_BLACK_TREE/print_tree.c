#include "rbt.h"

void print_tree(tree_t *root)
{
    /* Definition goes here */
    if (root == NULL)
        return;     
    print_tree(root->left);
    if (root->color == 1) 
    {
        printf("(%d)--(RED->1)  ", root->data);
    }
    else 
    {
        printf("(%d)--(BLACK->0)  ", root->data);
    }
    print_tree(root->right);
}