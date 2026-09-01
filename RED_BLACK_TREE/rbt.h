
#ifndef RBT_H
#define RBT_H

#define SUCCESS 0
#define FAILURE -1
#define DUPLICATE 2
#define LIST_EMPTY 3
#define DATA_NOT_FOUND 4

#define RED   0
#define BLACK 1

#define RED_COLOR     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"


#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

typedef struct node
{
	struct node *left;
	data_t data;
	struct node *right;
	int color;
	struct node *parent;
}tree_t;

/* keep all the prototypes of the functions here */
int create_a_tree(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);
int find_minimum(tree_t **root, data_t *min);
int delete_minimum(tree_t **root);
int find_maximum(tree_t **root, data_t *max);
int delete_maximum(tree_t **root);
tree_t *search(tree_t *root, data_t item);

void print_tree(tree_t *root);
void left_rotate(tree_t **root, tree_t *x);
void right_rotate(tree_t **root, tree_t *y);
void fix_insert(tree_t **root, tree_t *node);
tree_t *successor(tree_t *node);
void fix_delete(tree_t **root, tree_t *x);

#endif
