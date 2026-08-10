#include "rbt.h"

/*
 * Create a new Red-Black Tree node.
 */
tree_t *create_node(data_t item)
{
    tree_t *new_node;

    new_node = malloc(sizeof(tree_t));

    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->left = NULL;
    new_node->data = item;
    new_node->right = NULL;

    /* New nodes are initially RED */
    new_node->color = RED;

    return new_node;
}


/*
 * Find the parent of a given node.
 *
 * Since our tree node does not contain a parent pointer,
 * we find the parent by traversing from the root.
 */
static tree_t *find_parent(tree_t *root, tree_t *node)
{
    tree_t *current;

    if (root == NULL || node == NULL || root == node)
    {
        return NULL;
    }

    current = root;

    while (current != NULL)
    {
        if (current->left == node || current->right == node)
        {
            return current;
        }

        if (node->data < current->data)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return NULL;
}


/*
 * Left rotation.
 */
void left_rotate(tree_t **root, tree_t *x)
{
    tree_t *y;
    tree_t *parent;

    if (root == NULL || *root == NULL || x == NULL)
    {
        return;
    }

    y = x->right;

    if (y == NULL)
    {
        return;
    }

    /* Find parent before changing the tree */
    parent = find_parent(*root, x);

    /* Move y's left subtree to x's right */
    x->right = y->left;

    /* Put x below y */
    y->left = x;

    /* Connect y to x's old parent */
    if (parent == NULL)
    {
        *root = y;
    }
    else if (parent->left == x)
    {
        parent->left = y;
    }
    else
    {
        parent->right = y;
    }
}


/*
 * Right rotation.
 */
void right_rotate(tree_t **root, tree_t *x)
{
    tree_t *y;
    tree_t *parent;

    if (root == NULL || *root == NULL || x == NULL)
    {
        return;
    }

    y = x->left;

    if (y == NULL)
    {
        return;
    }

    /* Find parent before changing the tree */
    parent = find_parent(*root, x);

    /* Move y's right subtree to x's left */
    x->left = y->right;

    /* Put x below y */
    y->right = x;

    /* Connect y to x's old parent */
    if (parent == NULL)
    {
        *root = y;
    }
    else if (parent->left == x)
    {
        parent->left = y;
    }
    else
    {
        parent->right = y;
    }
}


/*
 * Restore Red-Black Tree properties after insertion.
 */
void insert_fixup(tree_t **root, tree_t *node)
{
    tree_t *parent;
    tree_t *grandparent;
    tree_t *uncle;

    while (node != *root)
    {
        parent = find_parent(*root, node);

        if (parent == NULL || parent->color == BLACK)
        {
            break;
        }

        grandparent = find_parent(*root, parent);

        if (grandparent == NULL)
        {
            break;
        }

        /*
         * Parent is the left child of grandparent.
         */
        if (parent == grandparent->left)
        {
            uncle = grandparent->right;

            /*
             * Case 1:
             * Parent and uncle are RED.
             *
             * Recolor parent and uncle BLACK,
             * and grandparent RED.
             */
            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                node = grandparent;
            }
            else
            {
                /*
                 * Case 2:
                 * Node is the right child.
                 *
                 * Convert it into the left-left case.
                 */
                if (node == parent->right)
                {
                    node = parent;
                    left_rotate(root, node);

                    parent = find_parent(*root, node);

                    if (parent == NULL)
                    {
                        break;
                    }

                    grandparent = find_parent(*root, parent);

                    if (grandparent == NULL)
                    {
                        break;
                    }
                }

                /*
                 * Case 3:
                 * Left-left case.
                 */
                parent->color = BLACK;
                grandparent->color = RED;

                right_rotate(root, grandparent);
            }
        }
        else
        {
            /*
             * Mirror case:
             * Parent is the right child of grandparent.
             */
            uncle = grandparent->left;

            /*
             * Case 1:
             * Parent and uncle are RED.
             */
            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;

                node = grandparent;
            }
            else
            {
                /*
                 * Case 2:
                 * Node is the left child.
                 *
                 * Convert it into the right-right case.
                 */
                if (node == parent->left)
                {
                    node = parent;
                    right_rotate(root, node);

                    parent = find_parent(*root, node);

                    if (parent == NULL)
                    {
                        break;
                    }

                    grandparent = find_parent(*root, parent);

                    if (grandparent == NULL)
                    {
                        break;
                    }
                }

                /*
                 * Case 3:
                 * Right-right case.
                 */
                parent->color = BLACK;
                grandparent->color = RED;

                left_rotate(root, grandparent);
            }
        }
    }

    /*
     * Red-Black Tree property:
     * Root must always be BLACK.
     */
    if (*root != NULL)
    {
        (*root)->color = BLACK;
    }
}


/*
 * Insert a new value into the Red-Black Tree.
 */
int insert(tree_t **root, data_t item)
{
    tree_t *new_node;
    tree_t *parent = NULL;
    tree_t *current;

    if (root == NULL)
    {
        return FAILURE;
    }

    new_node = create_node(item);

    if (new_node == NULL)
    {
        return FAILURE;
    }

    /*
     * If tree is empty, new node becomes root.
     */
    if (*root == NULL)
    {
        new_node->color = BLACK;
        *root = new_node;

        return SUCCESS;
    }

    /*
     * Find the correct position using BST rules.
     */
    current = *root;

    while (current != NULL)
    {
        parent = current;

        if (item < current->data)
        {
            current = current->left;
        }
        else if (item > current->data)
        {
            current = current->right;
        }
        else
        {
            /*
             * Duplicate data is not allowed.
             */
            free(new_node);
            return FAILURE;
        }
    }

    /*
     * Attach the new node to its parent.
     */
    if (item < parent->data)
    {
        parent->left = new_node;
    }
    else
    {
        parent->right = new_node;
    }

    /*
     * Restore Red-Black Tree properties.
     */
    insert_fixup(root, new_node);

    return SUCCESS;
}