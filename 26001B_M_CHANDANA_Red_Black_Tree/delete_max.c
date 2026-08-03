#include "main.h"

/* function definition to delete the maximum node in RED BLACK Tree */
int delete_maximum(tree_t **root)
{
    /*if tree is empty*/
    if (*root == NULL)
    {
        return FAILURE;
    }

    tree_t *temp = *root;
    tree_t *parent = NULL;
    tree_t *child = NULL;

    // find the right most node(maximum node)
    while (temp->right != NULL)
    {
        parent = temp;
        temp = temp->right;
    }

    // store the left child
    child = temp->left;

    // Root is maximum
    if (parent == NULL)
    {
        *root = child;
        if (child != NULL)
        {
            child->parent = NULL;
        }
    }

    // maximum is not the root node
    else
    {
        parent->right = child;
        if (child != NULL)
        {
            child->parent = parent;
        }
    }

    // save the maximum node color
    int deleted_color = temp->color;

    // delete maximum node
    free(temp);

    if (deleted_color == BLACK)
    {
        // call delete fix up
        if (*root != NULL)
            delete_fixup(root, child, parent);
    }

    return SUCCESS;
}