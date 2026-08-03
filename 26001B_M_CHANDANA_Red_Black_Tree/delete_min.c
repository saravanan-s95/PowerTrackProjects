#include "main.h"

/* function definition to delete the minimum node in RED BLACK Tree */
int delete_minimum(tree_t **root)
{
    /*if tree is empty*/
    if (*root == NULL)
    {
        return FAILURE;
    }

    tree_t *temp = *root;
    tree_t *parent = NULL;
    tree_t *child = NULL;

    // find the left most node(minimum node)
    while (temp->left != NULL)
    {
        parent = temp;
        temp = temp->left;
    }

    // store the right child
    child = temp->right;

    // Root is minimum
    if (parent == NULL)
    {
        *root = child;
        if (child != NULL)
        {
            child->parent = NULL;
        }
    }

    // minimum is not the root node
    else
    {
        parent->left = child;
        if (child != NULL)
        {
            child->parent = parent;
        }
    }

    // save the minimum node color
    int deleted_color = temp->color;

    // delete minimum node
    free(temp);

    if (deleted_color == BLACK)
    {
        // call delete fix up
        if (*root != NULL)
            delete_fixup(root, child, parent);
    }

    return SUCCESS;
}