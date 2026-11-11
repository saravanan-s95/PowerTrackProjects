#ifndef MAIN_H
#define MAIN_H

#define SUCCESS  0
#define FAILURE  -1
#define RED      0
#define BLACK    1

typedef int data_t;

typedef struct node
{
    data_t data;
    int colour;
    struct node *left, *right, *parent;

}tree_t;

extern tree_t *NIL;

//create node
tree_t * create_node(data_t item);

// Initialize
void init_tree(tree_t **root);

//insert operation
int insertion(tree_t **root, data_t item);

//insertion fix
void insertion_fix(tree_t **root, tree_t * z);

//right rotate
void right_rotate(tree_t **root, tree_t *x);

//left rotate
void left_rotate(tree_t **root, tree_t *x);

//search operation
tree_t *search(tree_t *root, data_t item);

//find minimum
int find_minimum(tree_t **root, data_t *min);

//find maximum
int find_maximum(tree_t **root, data_t *max);

//delete
int delete(tree_t **root, data_t item);

//transplant
void transplant(tree_t **root, tree_t *u, tree_t *v);

//delte fix
void delete_fix(tree_t **root, tree_t *t);

//tree minimum
tree_t *tree_minimum(tree_t *root);

//delete minimum
int delete_minimum(tree_t **root);

//delete maximum
int delete_maximum(tree_t **root);

//siblings
tree_t *siblings(tree_t *node);

//parent
tree_t *parent(tree_t *node);

//inorder
void inorder_display(tree_t *root);


#endif