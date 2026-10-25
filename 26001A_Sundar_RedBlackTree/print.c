#include "rbt.h"

void print_tree(tree_t* root)
{
    if(root == NULL)
    {
        return;
    }

    print_tree(root->left);

    printf("(%d)--(%s) ",root->data,root->color == RED ? "RED->0" : "BLACK->1");

    print_tree(root->right);
}