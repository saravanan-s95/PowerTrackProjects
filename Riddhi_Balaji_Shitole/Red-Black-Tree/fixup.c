#include "rbt.h"

void insert_fixup(tree_t **root, tree_t *z)
{
    while (z != *root &&
           z->parent != NULL &&
           z->parent->color == RED)
    {
        /* Parent is left child of grandparent */
        if (z->parent == z->parent->parent->left)
        {
            tree_t *uncle = z->parent->parent->right;

            /* Case 1: Uncle is RED */
            if (uncle != NULL && uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else
            {
                /* Case 2: Triangle */
                if (z == z->parent->right)
                {
                    z = z->parent;
                    left_rotate(root, z);
                }

                /* Case 3: Line */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                right_rotate(root, z->parent->parent);
            }
        }
        else
        {
            /* Mirror cases */
            tree_t *uncle = z->parent->parent->left;

            /* Mirror Case 1 */
            if (uncle != NULL && uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;

                z = z->parent->parent;
            }
            else
            {
                /* Mirror Case 2 */
                if (z == z->parent->left)
                {
                    z = z->parent;
                    right_rotate(root, z);
                }

                /* Mirror Case 3 */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                left_rotate(root, z->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}