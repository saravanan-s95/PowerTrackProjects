/***************************************************************************************************************************************************************
*Title      : This is the header file for the Red Black Tree
****************************************************************************************************************************************************************/

#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1

#define RED 0
#define BLACK 1

typedef int data_t;

typedef struct node
{
    struct node *left;
    data_t data;
    struct node *right;
    int color;
} tree_t;


/* Function prototypes */

/* Function prototypes */

int insert(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);

int find_minimum(tree_t **root, data_t *min);
int delete_minimum(tree_t **root);

int find_maximum(tree_t **root, data_t *max);
int delete_maximum(tree_t **root);

void print_tree(tree_t *root);
int search(tree_t *root, data_t item);
tree_t *create_node(data_t item);
void left_rotate(tree_t **root, tree_t *x);
void right_rotate(tree_t **root, tree_t *x);
void insert_fixup(tree_t **root, tree_t *node);

tree_t *find_node(tree_t *root, data_t item);
tree_t *minimum_node(tree_t *root);
tree_t *get_sibling(tree_t *root, tree_t *node);
void delete_fixup(tree_t **root, tree_t *node, tree_t *parent,
                  int node_is_left);
tree_t *find_parent_for_delete(tree_t *root, tree_t *node);
#endif