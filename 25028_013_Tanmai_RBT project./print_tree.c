#include "rbt.h"

void print_tree(tree_t *root)
{
    if (root == NULL)
    {
        return;
    }

    print_tree(root->left);

    if (root->color == RED)
    {
        printf("%d -- RED\n", root->data);
    }
    else
    {
        printf("%d -- BLACK\n", root->data);
    }

    print_tree(root->right);
}