#ifndef RBT_H
#define RBT_H

/* Importing required libraries */
#include <stdio.h>
#include <stdlib.h>

/* Macros */
#define SUCCESS 0
#define FAILURE -1
#define DUPLICATE -2

/* Enums */
typedef enum
{
    RED,
    BLACK
} Node_Color;

/* Structure of node */
typedef struct node
{
    int data;
    Node_Color color;
    struct node *left;
    struct node *right;
    struct node *parent;
} Tree;

/* Function prototypes */

/* Function to create a node */
Tree *create_node(int data);

/* Function to insert an element into tree*/
int insert(Tree **root, int data);

/* Function to display tree */
void display(Tree *root);

/* Function to print tree in Inorder manner */
void inOrder(Tree *root);

/* Function to perform "Left Rotate" operation */
void left_rotate(Tree **root, Tree *x);

/* Function to perform "Right Rotate" operation */
void right_rotate(Tree **root, Tree *y);

#endif