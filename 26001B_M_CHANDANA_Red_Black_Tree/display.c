#include "main.h"

void inorder(tree_t *root)
{
    if (root == NULL)
        return;

    inorder(root->left);

    printf("%d(%s) ", root->data, root->color == RED ? "R" : "B");

    inorder(root->right);
}