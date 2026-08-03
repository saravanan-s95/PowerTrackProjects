#include "main.h"

void delete_fixup(tree_t **root, tree_t *x, tree_t *parent)
{
    tree_t *sibling;

    while ((x != *root) && (x == NULL || x->color == BLACK))
    {
        if (x == parent->left)
        {
            sibling = parent->right;

            /*if sibling color is red*/
            if (sibling->color == RED)
            {
                parent->color = RED;
                sibling->color = BLACK;
                left_rotate(root, parent);
                sibling = parent->right;
            }

            /*if sibling color is black*/
            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK))
            {
                sibling->color = RED;

                x = parent;
                parent = x->parent;
            }

            /*sibling is BLACK, near child is RED, far child is BLACK*/
            else if ((sibling->right == NULL || sibling->right->color == BLACK) &&
                     (sibling->left != NULL && sibling->left->color == RED))
            {
                sibling->color = RED;
                sibling->left->color = BLACK;
                right_rotate(root, sibling);
                sibling = parent->right;
            }

            /*sibling is BLACK, near child is BLACK, far child is RED*/
            else
            {
                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->right != NULL)
                    sibling->right->color = BLACK;
                left_rotate(root, parent);
                x = *root;
            }
        }
        else
        {
            sibling = parent->left;
            /*if the sibling is RED*/
            if (sibling->color == RED)
            {
                parent->color = RED;
                sibling->color = BLACK;
                right_rotate(root, parent);
                sibling = parent->left;
            }

            /*if sibling color is black*/
            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK))
            {
                sibling->color = RED;

                x = parent;
                parent = x->parent;
            }

            /*sibling is BLACK, near child is RED, far child is BLACK*/
            else if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                     (sibling->right != NULL && sibling->right->color == RED))
            {
                sibling->color = RED;
                sibling->right->color = BLACK;
                left_rotate(root, sibling);
                sibling = parent->left;
            }

            /*sibling is BLACK, near child is BLACK, far child is RED*/
            else
            {
                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->left != NULL)
                    sibling->left->color = BLACK;
                right_rotate(root, parent);
                x = *root;
            }
        }
    }

    if (x != NULL)
    {
        x->color = BLACK;
    }
}