/*header guard*/
#ifndef MAIN_H
#define MAIN_H

#define SUCCESS 0
#define FAILURE -1

/*header files*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef int data_t;

typedef struct node
{
    data_t data;
    int color;

    struct node *left;
    struct node *right;
    struct node *parent;

} tree_t;

#define RED 0
#define BLACK 1

/* function declaration for inserting the node */
int insert(tree_t **root, data_t item);

/* function declaration for deleting the node */
int delete (tree_t **root, data_t item);

/* function declaration to find the minimum node */
int find_minimum(tree_t **root, data_t *min);

/* function declaration to find the maximum node */
int find_maximum(tree_t **root, data_t *max);

/* function declaration to delete the minimum node */
int delete_minimum(tree_t **root);

/* function declaration to delete the maximum node */
int delete_maximum(tree_t **root);

/*function declaration for insert fixup */
void insert_fixup(tree_t **root, tree_t *new);

/* function declaration for left rotation */
void left_rotate(tree_t **root, tree_t *x);

/* function declaration for right rotation */
void right_rotate(tree_t **root, tree_t *x);

/* function declaration for validating the input from the user */
int validate_input(char *str);

void inorder(tree_t *root);

/*function declaration for delete fixup */
void delete_fixup(tree_t **root, tree_t *x, tree_t *parent);

/*function declaration for validating the integer*/
int read_integer(data_t *num);

#endif
