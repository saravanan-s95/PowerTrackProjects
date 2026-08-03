/***************************************************************************************************************************************************************
*Title		: This the header file for the Red Black Tree
****************************************************************************************************************************************************************/
#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS   0
#define FAILURE  -1
#define DUPLICATE -2

#define RED   0
#define BLACK 1

typedef int data_t;

typedef struct node
{
    data_t data;
    int color;

    struct node *left;
    struct node *right;
    struct node *parent;

} tree_t;

/* Core Operations */
int insert(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);

int find_minimum(tree_t **root, data_t *min);
int find_maximum(tree_t **root, data_t *max);

int delete_minimum(tree_t **root);
int delete_maximum(tree_t **root);

/* Display */
void print_tree(tree_t *root);

/* Rotations */
void left_rotate(tree_t **root, tree_t *x);
void right_rotate(tree_t **root, tree_t *y);

/* Fixups */
void insert_fixup(tree_t **root, tree_t *z);
void delete_fixup(tree_t **root, tree_t *x);

/* Search */
tree_t *search(tree_t *root, data_t key);

/* Helpers */
tree_t *tree_minimum(tree_t *root);
tree_t *tree_maximum(tree_t *root);

#endif