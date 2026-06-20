#include "rbt.h"

int delete(tree_t **root, data_t item)
{
    if (*root == NULL)
        return FAILURE;

    tree_t *temp = *root;
    tree_t *parent = NULL;

    /* Search node */
    while (temp && temp->data != item)
    {
        parent = temp;

        if (item < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (temp == NULL)
        return FAILURE;

    /* Case 1: No child */
    if (temp->left == NULL && temp->right == NULL)
    {
        if (parent == NULL)
            *root = NULL;
        else if (parent->left == temp)
            parent->left = NULL;
        else
            parent->right = NULL;

        free(temp);
    }

    /* Case 2: One child */
    else if (temp->left == NULL || temp->right == NULL)
    {
        tree_t *child = (temp->left) ? temp->left : temp->right;

        if (parent == NULL)
            *root = child;
        else if (parent->left == temp)
            parent->left = child;
        else
            parent->right = child;

        free(temp);
    }

    /* Case 3: Two children */
    else
    {
        tree_t *succ_parent = temp;
        tree_t *succ = temp->right;

        while (succ->left)
        {
            succ_parent = succ;
            succ = succ->left;
        }

        printf("successor data: %d\n", succ->data);

        temp->data = succ->data;

        if (succ_parent->left == succ)
            succ_parent->left = succ->right;
        else
            succ_parent->right = succ->right;

        free(succ);
    }

    return SUCCESS;
}