#include "rbt.h"

/*
 * Find a node containing the given data.
 */
tree_t *find_node(tree_t *root, data_t item)
{
    tree_t *current = root;

    while (current != NULL)
    {
        if (item == current->data)
        {
            return current;
        }
        else if (item < current->data)
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
 * Find the minimum node in a subtree.
 */
tree_t *minimum_node(tree_t *root)
{
    tree_t *current = root;

    if (current == NULL)
    {
        return NULL;
    }

    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}


/*
 * Find the parent of a node.
 */
tree_t *find_parent_for_delete(tree_t *root, tree_t *node)
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
 * Find the sibling of a node.
 */
tree_t *get_sibling(tree_t *root, tree_t *node)
{
    tree_t *parent;

    if (root == NULL || node == NULL || root == node)
    {
        return NULL;
    }

    parent = find_parent_for_delete(root, node);

    if (parent == NULL)
    {
        return NULL;
    }

    if (parent->left == node)
    {
        return parent->right;
    }

    return parent->left;
}


/*
 * Red-Black Tree deletion fix-up.
 *
 * node_is_left tells us whether a NULL node represents
 * the left child or right child of parent.
 */
void delete_fixup(tree_t **root, tree_t *node, tree_t *parent,
                  int node_is_left)
{
    tree_t *sibling;

    while ((node == NULL || node->color == BLACK) &&
           node != *root)
    {
        if (parent == NULL)
        {
            break;
        }

        /*
         * Node is the LEFT child.
         */
        if ((node != NULL && parent->left == node) ||
            (node == NULL && node_is_left))
        {
            sibling = parent->right;

            /*
             * Case 1:
             * Sibling is RED.
             */
            if (sibling != NULL && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;

                left_rotate(root, parent);

                sibling = parent->right;
            }

            /*
             * Case 2:
             * Sibling is BLACK and both children are BLACK.
             */
            if (sibling == NULL ||
                ((sibling->left == NULL ||
                  sibling->left->color == BLACK) &&
                 (sibling->right == NULL ||
                  sibling->right->color == BLACK)))
            {
                if (sibling != NULL)
                {
                    sibling->color = RED;
                }

                node = parent;

                parent = find_parent_for_delete(*root, node);

                if (parent != NULL)
                {
                    node_is_left = (parent->left == node);
                }
            }
            else
            {
                /*
                 * Case 3:
                 * Sibling is BLACK,
                 * near child is RED,
                 * far child is BLACK.
                 */
                if (sibling->right == NULL ||
                    sibling->right->color == BLACK)
                {
                    if (sibling->left != NULL)
                    {
                        sibling->left->color = BLACK;
                    }

                    sibling->color = RED;

                    right_rotate(root, sibling);

                    sibling = parent->right;
                }

                /*
                 * Case 4:
                 * Sibling's far child is RED.
                 */
                if (sibling != NULL)
                {
                    sibling->color = parent->color;
                }

                parent->color = BLACK;

                if (sibling != NULL &&
                    sibling->right != NULL)
                {
                    sibling->right->color = BLACK;
                }

                left_rotate(root, parent);

                node = *root;
                parent = NULL;
            }
        }
        else
        {
            /*
             * Node is the RIGHT child.
             * Mirror of the above cases.
             */
            sibling = parent->left;

            /*
             * Case 1:
             * Sibling is RED.
             */
            if (sibling != NULL && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;

                right_rotate(root, parent);

                sibling = parent->left;
            }

            /*
             * Case 2:
             * Sibling is BLACK and both children are BLACK.
             */
            if (sibling == NULL ||
                ((sibling->left == NULL ||
                  sibling->left->color == BLACK) &&
                 (sibling->right == NULL ||
                  sibling->right->color == BLACK)))
            {
                if (sibling != NULL)
                {
                    sibling->color = RED;
                }

                node = parent;

                parent = find_parent_for_delete(*root, node);

                if (parent != NULL)
                {
                    node_is_left = (parent->left == node);
                }
            }
            else
            {
                /*
                 * Case 3:
                 * Sibling is BLACK,
                 * near child is RED,
                 * far child is BLACK.
                 */
                if (sibling->left == NULL ||
                    sibling->left->color == BLACK)
                {
                    if (sibling->right != NULL)
                    {
                        sibling->right->color = BLACK;
                    }

                    sibling->color = RED;

                    left_rotate(root, sibling);

                    sibling = parent->left;
                }

                /*
                 * Case 4:
                 * Sibling's far child is RED.
                 */
                if (sibling != NULL)
                {
                    sibling->color = parent->color;
                }

                parent->color = BLACK;

                if (sibling != NULL &&
                    sibling->left != NULL)
                {
                    sibling->left->color = BLACK;
                }

                right_rotate(root, parent);

                node = *root;
                parent = NULL;
            }
        }
    }

    if (node != NULL)
    {
        node->color = BLACK;
    }

    if (*root != NULL)
    {
        (*root)->color = BLACK;
    }
}


/*
 * Delete a node from the Red-Black Tree.
 */
int delete(tree_t **root, data_t item)
{
    tree_t *node;
    tree_t *remove_node;
    tree_t *child;
    tree_t *parent;
    int original_color;
    int node_is_left;

    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    /*
     * Find the node to delete.
     */
    node = find_node(*root, item);

    if (node == NULL)
    {
        return FAILURE;
    }

    /*
     * If node has two children,
     * use its inorder successor.
     */
    if (node->left != NULL && node->right != NULL)
    {
        remove_node = minimum_node(node->right);

        /*
         * Copy successor data into the node.
         */
        node->data = remove_node->data;
    }
    else
    {
        remove_node = node;
    }

    /*
     * Save the color before removing the node.
     */
    original_color = remove_node->color;

    /*
     * Get the child that will replace remove_node.
     */
    if (remove_node->left != NULL)
    {
        child = remove_node->left;
    }
    else
    {
        child = remove_node->right;
    }

    /*
     * Find the parent before changing the links.
     */
    parent = find_parent_for_delete(*root, remove_node);

    /*
     * Remember whether remove_node is the left child.
     * This is important when child is NULL.
     */
    if (parent != NULL)
    {
        node_is_left = (parent->left == remove_node);
    }
    else
    {
        node_is_left = 0;
    }

    /*
     * Remove the node from the tree.
     */
    if (parent == NULL)
    {
        *root = child;
    }
    else if (parent->left == remove_node)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }

    /*
     * If replacement child exists, it takes the removed
     * node's position.
     */
    if (child != NULL)
    {
        child->color = BLACK;
    }

    free(remove_node);

    /*
     * If a BLACK node was removed and there is no RED child
     * to absorb the extra black, perform fix-up.
     */
    if (original_color == BLACK)
    {
        if (child != NULL)
        {
            child->color = BLACK;
        }
        else
        {
            delete_fixup(root, child, parent, node_is_left);
        }
    }

    return SUCCESS;
}