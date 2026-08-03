/*******************************************************************************************************************************************************************
*Title             : Insertion
*Description       : This function performs inserting the new data into the given Red Black tree.
*Prototype         : int insert(tree_t **root, data_t item);
*Input Parameters  : root – Pointer to the root node of the Red Black tree.
                    : item – New data to be inserted into the Red Black tree.
*Output            : Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int insert(tree_t **root, data_t item)
{
    tree_t *new = malloc(sizeof(tree_t));

    if (new == NULL)
        return FAILURE;

    new->data = item;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    new->color = RED;

    if (*root == NULL)
    {
        new->color = BLACK;
        *root = new;

        printf("\nNow Tree is balance\n");
        print_tree(*root);

        return SUCCESS;
    }

    tree_t *temp = *root;
    tree_t *prev = NULL;

    while (temp)
    {
        prev = temp;

        if (item < temp->data)
            temp = temp->left;
        else if (item > temp->data)
            temp = temp->right;
        else
        {
            free(new);
            return DUPLICATE;
        }
    }

    new->parent = prev;

    if (item < prev->data)
        prev->left = new;
    else
        prev->right = new;

        printf("\nBefore Balancing Red Black Tree is :\n");
        print_tree(*root);

        insert_fixup(root, new);

        printf("\nNow Tree is balance\n");
        print_tree(*root);
 
        return SUCCESS;
}