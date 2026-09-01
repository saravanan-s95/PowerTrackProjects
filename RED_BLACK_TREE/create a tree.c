
#include "rbt.h"

int create_a_tree(tree_t **root, data_t item)
{
	/* Allocate memory for the new node */
	tree_t *new = malloc(sizeof(tree_t));

	if(new == NULL)
	{
		return FAILURE;
	}

	new->data = item;          // Store the data
    new->color = RED;          // Every newly inserted node is RED
    new->left = NULL;          // Left child is NULL
    new->right = NULL;         // Right child is NULL
    new->parent = NULL;        // Parent is initially NULL


	/* Start traversing from the root */
    tree_t *temp = *root;

    /* Parent pointer to keep track of parent node */
    tree_t *parent = NULL;

	while(temp != NULL)
	{
		/* Store current node as parent */
        parent = temp;

        /* Move towards left subtree */
        if (item < temp->data)
        {
            temp = temp->left;
        }

        /* Move towards right subtree */
        else if (item > temp->data)
        {
            temp = temp->right;
        }

        /* Duplicate node is not allowed */
        else
        {
            free(new);          // Release allocated memory
            return DUPLICATE;
        }
	}

	/* Set the parent of the new node */
    new->parent = parent;

    /* If tree is empty, new node becomes root */
    if (parent == NULL)
    {
        *root = new;

        /* Root node must always be BLACK */
        new->color = BLACK;
    }

    /* Insert as left child */
    else if (item < parent->data)
    {
        parent->left = new;
    }

    /* Insert as right child */
    else
    {
        parent->right = new;
    }

    /* Display tree before balancing */
    printf(GREEN "\nBefore Balancing Red Black Tree is:\n\n" RESET);

    /* Print the tree */
    print_tree(*root);

    /* Restore Red-Black Tree properties */
    fix_insert(root, new);

    /* Display balanced tree */
    printf(GREEN "\n\nNow Tree is balanced\n\n" RESET);

    /* Print balanced tree */
    print_tree(*root);


	return SUCCESS;

}
