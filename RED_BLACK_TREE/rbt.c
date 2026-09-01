#include "rbt.h"

void left_rotate(tree_t **root, tree_t *x)
{
    /* Store the right child of x */
    tree_t *y = x->right;

    /* Move y's left subtree to x's right subtree */
    x->right = y->left;

    /* Update the parent pointer of y's left child */
    if (y->left != NULL)
    {
        y->left->parent = x;
    }

    /* Make x's parent the parent of y */
    y->parent = x->parent;

    /* If x is the root, make y the new root */
    if (x->parent == NULL)
    {
        *root = y;
    }

    /* If x is the left child of its parent */
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }

    /* If x is the right child of its parent */
    else
    {
        x->parent->right = y;
    }

    /* Make x the left child of y */
    y->left = x;

    /* Update x's parent pointer */
    x->parent = y;
}


void right_rotate(tree_t **root, tree_t *y)
{
    /* Store the left child of y */
    tree_t *x = y->left;

    /* Move x's right subtree to y's left subtree */
    y->left = x->right;

    /* Update the parent pointer of x's right child */
    if (x->right != NULL)
    {
        x->right->parent = y;
    }

    /* Make y's parent the parent of x */
    x->parent = y->parent;

    /* If y is the root, make x the new root */
    if (y->parent == NULL)
    {
        *root = x;
    }

    /* If y is the left child of its parent */
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }

    /* If y is the right child of its parent */
    else
    {
        y->parent->right = x;
    }

    /* Make y the right child of x */
    x->right = y;

    /* Update y's parent pointer */
    y->parent = x;
}



void fix_insert(tree_t **root, tree_t *node)
{
    /* Declare pointers for parent, grandparent and uncle */
    tree_t *parent = NULL;
    tree_t *grandparent = NULL;
    tree_t *uncle = NULL;

    /* Continue fixing until Red-Black Tree properties are restored */
    while ((node != *root) && (node->color == RED) && (node->parent->color == RED))
    {
        /* Get parent and grandparent of the current node */
        parent = node->parent;
        grandparent = parent->parent;

        /************* Parent is Left Child *************/
        if (parent == grandparent->left)
        {
            /* Find the uncle node */
            uncle = grandparent->right;

            /* Case 1 : Uncle is RED */
            if (uncle != NULL && uncle->color == RED)
            {
                /* Recolor parent and uncle to BLACK */
                parent->color = BLACK;
                uncle->color = BLACK;

                /* Recolor grandparent to RED */
                grandparent->color = RED;

                /* Continue checking from grandparent */
                node = grandparent;
            }
            else
            {
                /* Case 2 : Left-Right (LR) */
                if (node == parent->right)
                {
                    /* Convert LR into LL by left rotation */
                    left_rotate(root, parent);

                    /* Update node and parent pointers */
                    node = parent;
                    parent = node->parent;
                }

                /* Case 3 : Left-Left (LL) */

                /* Perform right rotation on grandparent */
                right_rotate(root, grandparent);

                /* Swap colors of parent and grandparent */
                int temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;

                /* Move node upward */
                node = parent;
            }
        }

        /************* Parent is Right Child *************/
        else
        {
            /* Find the uncle node */
            uncle = grandparent->left;

            /* Mirror Case 1 : Uncle is RED */
            if (uncle != NULL && uncle->color == RED)
            {
                /* Recolor parent and uncle to BLACK */
                parent->color = BLACK;
                uncle->color = BLACK;

                /* Recolor grandparent to RED */
                grandparent->color = RED;

                /* Continue checking from grandparent */
                node = grandparent;
            }
            else
            {
                /* Mirror Case 2 : Right-Left (RL) */
                if (node == parent->left)
                {
                    /* Convert RL into RR by right rotation */
                    right_rotate(root, parent);

                    /* Update node and parent pointers */
                    node = parent;
                    parent = node->parent;
                }

                /* Mirror Case 3 : Right-Right (RR) */

                /* Perform left rotation on grandparent */
                left_rotate(root, grandparent);

                /* Swap colors of parent and grandparent */
                int temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;

                /* Move node upward */
                node = parent;
            }
        }
    }

    /* Root of a Red-Black Tree must always be BLACK */
    (*root)->color = BLACK;
}



void print_tree(tree_t *root)
{
    /* Check whether the tree is empty */
    if (root == NULL)
    {
        return;
    }

    /* Traverse the left subtree */
    print_tree(root->left);

    /* Print the current node's data and color
       R -> Red node
       B -> Black node */
    printf(GREEN "(%d)--(%s) " RESET, root->data, (root->color == RED) ? "RED->0" : "BLACK->1");

    /* Traverse the right subtree */
    print_tree(root->right);
}



tree_t *successor(tree_t *node)
{
    /* Check if the node is NULL */
    if (node == NULL)
    {
        return NULL;
    }

    /* Case 1: Node has a right subtree */
    if (node->right != NULL)
    {
        node = node->right;

        /* Find the leftmost node in the right subtree */
        while (node->left != NULL)
        {
            node = node->left;
        }

        return node;
    }

    /* Case 2: Node has no right subtree */
    tree_t *parent = node->parent;

    while (parent != NULL && node == parent->right)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

void fix_delete(tree_t **root, tree_t *x) 
{
    while (x != *root && (x == NULL || x->color == BLACK)) 
    {
        if (x == x->parent->left) 
        {
            tree_t *w = x->parent->right;
            if (w->color == RED) 
            {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) 
            {
                w->color = RED;
                x = x->parent;
            } 
            else 
            {
                if (w->right == NULL || w->right->color == BLACK) 
                {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                left_rotate(root, x->parent);
                x = *root;
            }
        } 
        else 
        {
            tree_t *w = x->parent->left;
            if (w->color == RED) 
            {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || w->right->color == BLACK) &&
                (w->left == NULL || w->left->color == BLACK)) 
            {
                w->color = RED;
                x = x->parent;
            } 
            else 
            {
                if (w->left == NULL || w->left->color == BLACK) 
                {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    if (x != NULL) x->color = BLACK;
}
