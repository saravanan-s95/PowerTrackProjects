#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void left_rotate(RBTree *tree, RBNode *x)
{
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void right_rotate(RBTree *tree, RBNode *y)
{
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->nil)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == tree->nil)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

bool rbt_init(RBTree *tree)
{
    if (!tree)
        return false;

    tree->nil = malloc(sizeof(*tree->nil));
    if (!tree->nil)
        return false;

    tree->nil->key = 0;
    tree->nil->color = BLACK;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->parent = tree->nil;
    tree->root = tree->nil;
    return true;
}

static void destroy_nodes(RBTree *tree, RBNode *node)
{
    if (node == tree->nil)
        return;
    destroy_nodes(tree, node->left);
    destroy_nodes(tree, node->right);
    free(node);
}

void rbt_destroy(RBTree *tree)
{
    if (!tree || !tree->nil)
        return;
    destroy_nodes(tree, tree->root);
    free(tree->nil);
    tree->nil = NULL;
    tree->root = NULL;
}

static RBNode *find_node(const RBTree *tree, int key)
{
    RBNode *current = tree->root;
    while (current != tree->nil && current->key != key) {
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }
    return current;
}

bool rbt_search(const RBTree *tree, int key)
{
    if (!tree || !tree->nil)
        return false;
    return find_node(tree, key) != tree->nil;
}

static void insert_fixup(RBTree *tree, RBNode *z)
{
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            RBNode *uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

bool rbt_insert(RBTree *tree, int key)
{
    if (!tree || !tree->nil)
        return false;

    RBNode *parent = tree->nil;
    RBNode *current = tree->root;

    while (current != tree->nil) {
        parent = current;
        if (key == current->key)
            return false; /* No duplicate keys. */
        current = (key < current->key) ? current->left : current->right;
    }

    RBNode *z = malloc(sizeof(*z));
    if (!z)
        return false;

    z->key = key;
    z->color = RED;
    z->left = tree->nil;
    z->right = tree->nil;
    z->parent = parent;

    if (parent == tree->nil)
        tree->root = z;
    else if (key < parent->key)
        parent->left = z;
    else
        parent->right = z;

    insert_fixup(tree, z);
    return true;
}

static RBNode *subtree_minimum(const RBTree *tree, RBNode *node)
{
    while (node->left != tree->nil)
        node = node->left;
    return node;
}

static RBNode *subtree_maximum(const RBTree *tree, RBNode *node)
{
    while (node->right != tree->nil)
        node = node->right;
    return node;
}

static void transplant(RBTree *tree, RBNode *u, RBNode *v)
{
    if (u->parent == tree->nil)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    /* Keep NIL's parent updated; deletion fix-up relies on it. */
    v->parent = u->parent;
}

static void delete_fixup(RBTree *tree, RBNode *x)
{
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                if (w != tree->nil)
                    w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                if (w != tree->nil)
                    w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

bool rbt_delete(RBTree *tree, int key)
{
    if (!tree || !tree->nil)
        return false;

    RBNode *z = find_node(tree, key);
    if (z == tree->nil)
        return false;

    RBNode *y = z;
    Color original_color = y->color;
    RBNode *x;

    if (z->left == tree->nil) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = subtree_minimum(tree, z->right);
        original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);
    if (original_color == BLACK)
        delete_fixup(tree, x);

    if (tree->root != tree->nil) {
        tree->root->color = BLACK;
        tree->root->parent = tree->nil;
    }
    return true;
}

bool rbt_minimum(const RBTree *tree, int *out_key)
{
    if (!tree || !tree->nil || !out_key || tree->root == tree->nil)
        return false;
    *out_key = subtree_minimum(tree, tree->root)->key;
    return true;
}

bool rbt_maximum(const RBTree *tree, int *out_key)
{
    if (!tree || !tree->nil || !out_key || tree->root == tree->nil)
        return false;
    *out_key = subtree_maximum(tree, tree->root)->key;
    return true;
}

bool rbt_delete_minimum(RBTree *tree, int *out_key)
{
    if (!tree || !tree->nil || !out_key || tree->root == tree->nil)
        return false;
    RBNode *node = subtree_minimum(tree, tree->root);
    *out_key = node->key;
    return rbt_delete(tree, node->key);
}

bool rbt_delete_maximum(RBTree *tree, int *out_key)
{
    if (!tree || !tree->nil || !out_key || tree->root == tree->nil)
        return false;
    RBNode *node = subtree_maximum(tree, tree->root);
    *out_key = node->key;
    return rbt_delete(tree, node->key);
}

static void display_inorder(const RBTree *tree, RBNode *node)
{
    if (node == tree->nil)
        return;
    display_inorder(tree, node->left);
    printf("(%d)--(%s) ", node->key, node->color == RED ? "RED" : "BLACK");
    display_inorder(tree, node->right);
}

void rbt_display(const RBTree *tree)
{
    if (!tree || !tree->nil) {
        puts("Tree is not initialized.");
        return;
    }
    if (tree->root == tree->nil) {
        puts("Tree is empty.");
        return;
    }
    printf("In-order (sorted): ");
    display_inorder(tree, tree->root);
    putchar('\n');
    printf("Root: %d (%s)\n", tree->root->key,
           tree->root->color == RED ? "RED" : "BLACK");
}

static int validate_node(const RBTree *tree, RBNode *node,
                         long long low, long long high, bool *ok)
{
    if (node == tree->nil)
        return 1; /* NIL is black. */

    if (node->key <= low || node->key >= high)
        *ok = false;
    if (node->color == RED &&
        (node->left->color == RED || node->right->color == RED))
        *ok = false;
    if (node->left != tree->nil && node->left->parent != node)
        *ok = false;
    if (node->right != tree->nil && node->right->parent != node)
        *ok = false;

    int left_height = validate_node(tree, node->left, low, node->key, ok);
    int right_height = validate_node(tree, node->right, node->key, high, ok);
    if (left_height != right_height)
        *ok = false;

    return left_height + (node->color == BLACK ? 1 : 0);
}

bool rbt_validate(const RBTree *tree)
{
    if (!tree || !tree->nil || tree->nil->color != BLACK)
        return false;
    if (tree->root == tree->nil)
        return true;
    if (tree->root->color != BLACK || tree->root->parent != tree->nil)
        return false;

    bool ok = true;
    (void)validate_node(tree, tree->root, LLONG_MIN, LLONG_MAX, &ok);
    return ok;
}
