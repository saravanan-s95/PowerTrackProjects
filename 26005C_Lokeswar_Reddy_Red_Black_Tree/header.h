#ifndef RBT_H
#define RBT_H

#include <stddef.h>

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
	struct node *parent;
	int color;
}  tree_t;

int start(void);

int insert(tree_t **root, data_t item);
int search(tree_t *root, data_t item);
int delete(tree_t **root, data_t item);
void print_tree(tree_t *root);

int find_minimum(tree_t **root, data_t *min);
int find_maximum(tree_t **root, data_t *max);

int delete_minimum(tree_t **root);
int delete_maximum(tree_t **root);

void left_rotate(tree_t **root, tree_t *node);
void right_rotate(tree_t **root, tree_t *node);

#endif
