#include "rbt.h"

int delete(tree_t **root, data_t item)
{
    tree_t *temp = *root;
    tree_t *parent = NULL;

    while (temp != NULL && temp->data != item) 
    {
        parent = temp;

        if (item < temp->data)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }

    if (temp == NULL)
    {
        return DATA_NOT_FOUND;
    }

    // Find inorder successor BEFORE deletion
    tree_t *next = successor(temp);

    if (next != NULL) 
    {
        printf(GREEN "\nNext node is : (%d)--(%s)\n" RESET, next->data, (next->color == RED) ? "RED->0" : "BLACK->1");
    } 
    else 
    {
        printf(YELLOW "No next node exists.\n" RESET);
    }

    int deleted_color = temp->color;
    tree_t *child = NULL;

    /******** Case 1: Leaf ********/
    if (temp->left == NULL && temp->right == NULL) 
    {
        if (parent == NULL) 
        {
            *root = NULL;
        } 
        else if (parent->left == temp) 
        {
            parent->left = NULL;
        } 
        else 
        {
            parent->right = NULL;
        }
    }

    /******** Case 2: Only Left ********/
    else if (temp->right == NULL) 
    {
        child = temp->left;
        if (parent == NULL) 
        {
            *root = child;
            child->parent = NULL;
        } 
        else if (parent->left == temp) 
        {
            parent->left = child;
            child->parent = parent;
        } 
        else 
        {
            parent->right = child;
            child->parent = parent;
        }
    }

    /******** Case 3: Only Right ********/
    else if (temp->left == NULL) 
    {
        child = temp->right;

        if (parent == NULL) 
        {
            *root = child;
            child->parent = NULL;
        } 
        else if (parent->left == temp) 
        {
            parent->left = child;
            child->parent = parent;
        } 
        else 
        {
            parent->right = child;
            child->parent = parent;
        }
    }

    /******** Case 4: Two Children ********/
    else 
    {
        tree_t *succ = temp->right;
        tree_t *succ_parent = temp;

        while (succ->left != NULL) 
        {
            succ_parent = succ;
            succ = succ->left;
        }

        temp->data = succ->data;
        deleted_color = succ->color;
        child = succ->right;

        if (succ_parent->left == succ)
        {
            succ_parent->left = child;
        }
        else
        {
            succ_parent->right = child;
        }

        if (child != NULL)
        {
            child->parent = succ_parent;
        }

        free(succ);
        goto FIX;
    }

    free(temp);

FIX:
    if (deleted_color == BLACK) 
    {
        fix_delete(root, (child != NULL) ? child : parent);
    }

    return SUCCESS;
}
