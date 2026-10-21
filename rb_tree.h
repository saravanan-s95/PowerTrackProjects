
#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef int data_t;

typedef enum
{
    RED,
    BLACK
} color_t;

typedef struct node
{
    data_t data;
    color_t color;
    struct node *left;
    struct node *right;
    struct node *parent;
} tree_t;

#define SUCCESS 0
#define FAILURE 1

int insert(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);

int search(tree_t *root, data_t item);

int find_minimum(tree_t **root, data_t *min);
int find_maximum(tree_t **root, data_t *max);

int delete_minimum(tree_t **root);
int delete_maximum(tree_t **root);

void display(tree_t *root);
void free_tree(tree_t *root);

#endif
