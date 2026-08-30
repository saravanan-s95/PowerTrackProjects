#include "rbt.h"

int insert(Tree **root, int data)
{
    // Creating a node with its default values
    Tree *new = create_node(data);

    // If allocation fails then return failure
    if (new == NULL)
    {
        printf("ERROR: Node cannot be created\n");
        return FAILURE;
    }

    // If tree is empty
    if (*root == NULL)
    {
        new->color = BLACK;
        *root = new;
        return SUCCESS;
    }

    // If tree is non-empty
    Tree *parent = NULL;
    Tree *temp = *root;

    // Traversing the tree
    while (temp)
    {
        parent = temp;
        if (data < temp->data)
            temp = temp->left;

        else if (data > temp->data)
            temp = temp->right;

        else
        {
            return DUPLICATE;
        }
    }

    // Now inserting the node
    if (data < parent->data)
    {
        new->parent = parent;
        parent->left = new;
    }

    else
    {
        new->parent = parent;
        parent->right = new;

        // R - R Conflict
        if (new->color == RED && new->parent->color == RED)
        {
            Tree *grandparent = new->parent->parent;
            Tree *uncle = grandparent->left;
            if (uncle == NULL || uncle->color == BLACK)
            {
                // Do rotation
                
            }
        }
    }

    return SUCCESS;
}