#include "header.h"

void left_rotate(tree_t **root, tree_t *node)
{
    tree_t *child;

    if (root == NULL || node == NULL || node->right == NULL)
        return;

    child = node->right;
    node->right = child->left;

    if (child->left != NULL)
        child->left->parent = node;

    child->parent = node->parent;

    if (node->parent == NULL)
        *root = child;
    else if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;

    child->left = node;
    node->parent = child;
}

void right_rotate(tree_t **root, tree_t *node)
{
    tree_t *child;

    if (root == NULL || node == NULL || node->left == NULL)
        return;

    child = node->left;
    node->left = child->right;

    if (child->right != NULL)
        child->right->parent = node;

    child->parent = node->parent;

    if (node->parent == NULL)
        *root = child;
    else if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;

    child->right  = node;
    node ->parent = child;
}