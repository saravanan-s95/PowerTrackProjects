#include "main.h"

/* function definition for deleting the node */
int delete(tree_t **root, data_t item)
{
    tree_t *temp;
    tree_t *successor;
    tree_t *child;
    tree_t *parent;

    /*tree is empty*/
    if(*root == NULL)
    {
        return FAILURE;
    }

    temp = *root;
    /*search the node*/
    while (temp != NULL)
    {
        if (item < temp->data)
            temp = temp->left;

        else if (item > temp->data)
            temp = temp->right;

        else
            break;
    }

    // node not found
    if (temp == NULL)
        return FAILURE;

    int deleted_color = temp->color;

    parent = temp->parent;

    /*No left child*/
    if (temp->left == NULL)
    {
        child = temp->right;
    }

    /*No right child*/
    else if (temp->right == NULL)
    {
        child = temp->left;
    }

    /*if both the nodes are present*/
    else
    {
        successor = temp->right;
        while(successor->left != NULL)
        {
            successor = successor->left;
        }

        /*copy the successor data*/
        temp->data = successor->data;

        temp = successor;

        parent = temp->parent;
        deleted_color = temp->color;
        child = temp->right;

    }

   

    /*deleting root node*/
    if (parent == NULL)
    {
        *root = child;
        if(child != NULL)
        {
            child->parent = NULL;
        }
    }

    /*if node is left child*/
    else if(temp == parent->left)
    {
        parent->left = child;
        if(child != NULL)
        {
            child->parent = parent;
        }
    }

    /*if node is right child*/
    else
    {
        parent->right = child;
        if(child != NULL)
        {
            child->parent = parent;
        }
    }

    free(temp);

    if (deleted_color == BLACK)
    {
        // call delete fix up
        if (*root != NULL)
            delete_fixup(root, child, parent);
    }

    return SUCCESS;

}