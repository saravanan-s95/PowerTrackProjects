#include "main.h"

/*function definition for inserting the node*/
int insert(tree_t **root, data_t item)
{
    /* create a new node using DMA */
    tree_t *new = malloc(sizeof(tree_t));
    if (new == NULL)
    {
        return FAILURE;
    }

    /*initialize the node*/
    new->data = item;
    new->color = RED;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;

    /* empty tree */
    if (*root == NULL)
    {
        new->color = BLACK; // root must be black
        *root = new;

        return SUCCESS;
    }

    /* non empty tree */
    tree_t *current = *root;
    tree_t *parent = NULL;

    while (current != NULL)
    {
        parent = current;
        if (item < current->data)
            current = current->left;

        else if (item > current->data)
            current = current->right;

        else
        {
            free(new);
            return FAILURE;
        }
    }

    new->parent = parent;

    if (item < parent->data)
        parent->left = new;

    else
        parent->right = new;

    /* fix the RED Black tree properties*/
    insert_fixup(root, new);

    return SUCCESS;
}
