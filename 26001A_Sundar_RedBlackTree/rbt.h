/***************************************************************************************************************************************************************
*Title		: This the header file for the Red Black Tree
****************************************************************************************************************************************************************/
#ifndef RBT_H
#define RBT_H

#define SUCCESS 2
#define FAILURE -1

#define RED 0
#define BLACK 1

#include<stdio.h>
#include<stdlib.h>

typedef int data_t;

typedef struct node
{
	struct node *left;
	data_t data;
	struct node *right;
	int color;
}tree_t;

/* keep all the prototypes of the functions here */

/* Insert in tree */
int insert(tree_t **root, data_t item);

/* Fix insert violoations */
void fix_violation(tree_t** root,tree_t* node);

/* LL rotation function */
void ll_rotation(tree_t** root,tree_t* greatgrandparent,tree_t* grandparent,tree_t* parent);

/* RR rotation function */
void rr_rotation(tree_t** root,tree_t* greatgrandparent,tree_t* grandparent,tree_t* parent);

/* Delete from tree */
int delete(tree_t **root, data_t item);

/* Fix deletion violations */
int delete_fix(tree_t** root,tree_t* parent,tree_t* sibling);

/* Find minimum element in tree */
int find_minimum(tree_t **root, data_t *min);

/* Find maximum element in tree */
int find_maximum(tree_t **root, data_t *max);

/* Delete minimum element in tree */
int delete_minimum(tree_t **root);

/* Delete maximum element in tree */
int delete_maximum(tree_t **root);

/* Search data in tree */
void search_data(tree_t* root,data_t item);

/* Print tree */
void print_tree(tree_t* root);

/* Free the memory before exiting the program */
void free_tree(tree_t *root);

#endif