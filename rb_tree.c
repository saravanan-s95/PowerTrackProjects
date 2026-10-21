
#include <stdio.h>
#include <stdlib.h>
#include "rb_tree.h"

static color_t get_color(tree_t *node)
{
    if (node == NULL)
        return BLACK;

    return node->color;
}

static tree_t *create_node(data_t item)
{
    tree_t *new_node = malloc(sizeof(tree_t));

    if (new_node == NULL)
        return NULL;

    new_node->data = item;
    new_node->color = RED;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;

    return new_node;
}

static void left_rotate(tree_t **root, tree_t *x)
{
    tree_t *y = x->right;

    x->right = y->left;

    if (y->left != NULL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(tree_t **root, tree_t *y)
{
    tree_t *x = y->left;

    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

static tree_t *find_node(tree_t *root, data_t item)
{
    while (root != NULL)
    {
        if (item == root->data)
            return root;

        if (item < root->data)
            root = root->left;
        else
            root = root->right;
    }

    return NULL;
}

int search(tree_t *root, data_t item)
{
    if (find_node(root, item) != NULL)
        return SUCCESS;

    return FAILURE;
}

static void insert_fix(tree_t **root, tree_t *node)
{
    while (node != *root &&
           get_color(node->parent) == RED)
    {
        tree_t *parent = node->parent;
        tree_t *grandparent = parent->parent;

        if (parent == grandparent->left)
        {
            tree_t *uncle = grandparent->right;

            if (get_color(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                if (node == parent->right)
                {
                    node = parent;
                    left_rotate(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                parent->color = BLACK;
                grandparent->color = RED;
                right_rotate(root, grandparent);
            }
        }
        else
        {
            tree_t *uncle = grandparent->left;

            if (get_color(uncle) == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                if (node == parent->left)
                {
                    node = parent;
                    right_rotate(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }

                parent->color = BLACK;
                grandparent->color = RED;
                left_rotate(root, grandparent);
            }
        }
    }

    if (*root != NULL)
        (*root)->color = BLACK;
}

int insert(tree_t **root, data_t item)
{
    tree_t *new_node;
    tree_t *parent = NULL;
    tree_t *current = *root;

    while (current != NULL)
    {
        parent = current;

        if (item == current->data)
            return FAILURE;

        if (item < current->data)
            current = current->left;
        else
            current = current->right;
    }

    new_node = create_node(item);

    if (new_node == NULL)
        return FAILURE;

    new_node->parent = parent;

    if (parent == NULL)
        *root = new_node;
    else if (item < parent->data)
        parent->left = new_node;
    else
        parent->right = new_node;

    insert_fix(root, new_node);

    return SUCCESS;
}

static void display_nodes(tree_t *root)
{
    if (root == NULL)
        return;

    display_nodes(root->left);

    if (root->parent == NULL)
        printf("(%d)--(%s->0) ", root->data,
               root->color == RED ? "RED" : "BLACK");
    else
        printf("(%d)--(%s->%d) ", root->data,
               root->color == RED ? "RED" : "BLACK",
               root->parent->data);

    display_nodes(root->right);
}

void display(tree_t *root)
{
    display_nodes(root);
}

static tree_t *minimum_node(tree_t *root)
{
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}

static tree_t *maximum_node(tree_t *root)
{
    if (root == NULL)
        return NULL;

    while (root->right != NULL)
        root = root->right;

    return root;
}

int find_minimum(tree_t **root, data_t *min)
{
    tree_t *node = minimum_node(*root);

    if (node == NULL)
        return FAILURE;

    *min = node->data;
    return SUCCESS;
}

int find_maximum(tree_t **root, data_t *max)
{
    tree_t *node = maximum_node(*root);

    if (node == NULL)
        return FAILURE;

    *max = node->data;
    return SUCCESS;
}

static void transplant(tree_t **root, tree_t *old_node,
                       tree_t *new_node)
{
    if (old_node->parent == NULL)
        *root = new_node;
    else if (old_node == old_node->parent->left)
        old_node->parent->left = new_node;
    else
        old_node->parent->right = new_node;

    if (new_node != NULL)
        new_node->parent = old_node->parent;
}

static void delete_fix(tree_t **root, tree_t *node,
                       tree_t *parent)
{
    tree_t *sibling;

    while (node != *root && get_color(node) == BLACK)
    {
        if (parent == NULL)
            break;

        if (node == parent->left)
        {
            sibling = parent->right;

            if (get_color(sibling) == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                left_rotate(root, parent);
                sibling = parent->right;
            }

            if (sibling == NULL)
            {
                node = parent;
                parent = node->parent;
            }
            else if (get_color(sibling->left) == BLACK &&
                     get_color(sibling->right) == BLACK)
            {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (get_color(sibling->right) == BLACK)
                {
                    if (sibling->left != NULL)
                        sibling->left->color = BLACK;

                    sibling->color = RED;
                    right_rotate(root, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->right != NULL)
                    sibling->right->color = BLACK;

                left_rotate(root, parent);
                node = *root;
                parent = NULL;
            }
        }
        else
        {
            sibling = parent->left;

            if (get_color(sibling) == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                right_rotate(root, parent);
                sibling = parent->left;
            }

            if (sibling == NULL)
            {
                node = parent;
                parent = node->parent;
            }
            else if (get_color(sibling->right) == BLACK &&
                     get_color(sibling->left) == BLACK)
            {
                sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (get_color(sibling->left) == BLACK)
                {
                    if (sibling->right != NULL)
                        sibling->right->color = BLACK;

                    sibling->color = RED;
                    left_rotate(root, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;

                if (sibling->left != NULL)
                    sibling->left->color = BLACK;

                right_rotate(root, parent);
                node = *root;
                parent = NULL;
            }
        }
    }

    if (node != NULL)
        node->color = BLACK;
}

int delete(tree_t **root, data_t item)
{
    tree_t *target = find_node(*root, item);
    tree_t *moved_node;
    tree_t *child;
    tree_t *child_parent;
    color_t original_color;

    if (target == NULL)
        return FAILURE;

    moved_node = target;
    original_color = moved_node->color;

    if (target->left == NULL)
    {
        child = target->right;
        child_parent = target->parent;
        transplant(root, target, target->right);
    }
    else if (target->right == NULL)
    {
        child = target->left;
        child_parent = target->parent;
        transplant(root, target, target->left);
    }
    else
    {
        moved_node = minimum_node(target->right);
        original_color = moved_node->color;
        child = moved_node->right;

        if (moved_node->parent == target)
        {
            child_parent = moved_node;

            if (child != NULL)
                child->parent = moved_node;
        }
        else
        {
            child_parent = moved_node->parent;
            transplant(root, moved_node, moved_node->right);

            moved_node->right = target->right;
            moved_node->right->parent = moved_node;
        }

        transplant(root, target, moved_node);

        moved_node->left = target->left;
        moved_node->left->parent = moved_node;
        moved_node->color = target->color;
    }

    free(target);

    if (original_color == BLACK)
        delete_fix(root, child, child_parent);

    if (*root != NULL)
        (*root)->color = BLACK;

    return SUCCESS;
}

int delete_minimum(tree_t **root)
{
    tree_t *node = minimum_node(*root);

    if (node == NULL)
        return FAILURE;

    return delete(root, node->data);
}

int delete_maximum(tree_t **root)
{
    tree_t *node = maximum_node(*root);

    if (node == NULL)
        return FAILURE;

    return delete(root, node->data);
}

void free_tree(tree_t *root)
{
    if (root == NULL)
        return;

    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
