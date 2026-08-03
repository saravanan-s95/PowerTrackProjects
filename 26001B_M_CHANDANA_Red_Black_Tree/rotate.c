#include "main.h"

/* function definition for left rotation */
void left_rotate(tree_t **root, tree_t *x)
{
    tree_t *y;
    y = x->right;

    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
    {
        *root = y;
    }

    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }

    else
    {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

/* function definition for right rotation */
void right_rotate(tree_t **root, tree_t *x)
{
    tree_t *y;
    y = x->left;

    x->left = y->right;

    if (y->right != NULL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
    {
        *root = y;
    }

    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }

    else
    {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
}