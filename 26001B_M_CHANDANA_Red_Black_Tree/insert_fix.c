#include "main.h"

void insert_fixup(tree_t **root, tree_t *new)
{
    tree_t *parent;
    tree_t *grandparent;
    tree_t *uncle;

    while (new != *root && new->parent->color == RED)
    {
        parent = new->parent;
        grandparent = parent->parent;

        if (parent == grandparent->left)
        {
            uncle = grandparent->right;

            // recolor
            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                new = grandparent;
            }

            // rotate and recolor
            else
            {
                if (new == parent->right)
                {
                    left_rotate(root, parent);

                    new = parent;
                    parent = new->parent;
                }
                right_rotate(root, grandparent);

                parent->color = BLACK;
                grandparent->color = RED;
            }
        }

        else
        {
            uncle = grandparent->left;

            // recolor
            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                new = grandparent;
            }

            // rotate and recolor
            else
            {
                if (new == parent->left)
                {
                    right_rotate(root, parent);

                    new = parent;
                    parent = new->parent;
                }
                left_rotate(root, grandparent);

                parent->color = BLACK;
                grandparent->color = RED;
            }
        }
    }

    /*root must be always BLACK*/
    (*root)->color = BLACK;
}