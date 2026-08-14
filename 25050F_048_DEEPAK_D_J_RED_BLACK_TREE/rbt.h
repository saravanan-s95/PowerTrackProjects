/***************************************************************************************************************************************************************
*Title		: This the header file for the Red Black Tree
****************************************************************************************************************************************************************/
#ifndef RBT_H
#define RBT_H
#define SUCCESS 0
#define FAILURE -1

#include <stdio.h>
#include <stdlib.h>
typedef int data_t;

typedef struct node
{
	struct node *left;
	data_t data;
	struct node *right;
	int color;
}tree_t;

/* keep all the prototypes of the functions here */
int insert(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);
int find_minimum(tree_t **root, data_t *min);
int delete_minimum(tree_t **root);
int find_maximum(tree_t **root, data_t *max);
int delete_maximum(tree_t **root);
void print_tree(tree_t *root);		
tree_t *find_parent(tree_t *root, tree_t *node);
void left_rotate(tree_t **root, tree_t *parent, tree_t *node);
void right_rotate(tree_t **root, tree_t *parent, tree_t *node);
#endif
