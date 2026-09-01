#include "rbt.h"

int delete_maximum(tree_t **root)
{
	if (*root == NULL)
    {
        return LIST_EMPTY;
    }

    tree_t *temp = *root;
    tree_t *parent = NULL;

    while (temp->right != NULL) 
    {
        parent = temp;
        temp = temp->right;
    }

    int deleted_color = temp->color;
    tree_t *child = temp->left;  // maximum node can only have left child

    if (parent == NULL) 
    {
        *root = child;
        if (child != NULL) 
        {
            child->parent = NULL;
        }
    } 
    else 
    {
        parent->right = child;
        if (child != NULL) 
        {
            child->parent = parent;
        }
    }

    free(temp);

    // Fix only if a BLACK node was removed
    if (deleted_color == BLACK) 
    {
        fix_delete(root, (child != NULL) ? child : parent);
    }

    return SUCCESS;
}
