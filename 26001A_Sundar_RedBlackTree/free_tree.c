#include "rbt.h"

void free_tree(tree_t *root)
{
    if(root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}