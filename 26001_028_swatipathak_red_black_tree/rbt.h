/***************************************************************************************************************************************************************
*Title		: This the header file for the Red Black Tree
****************************************************************************************************************************************************************/
#ifndef RBT_H
#define RBT_H
#include<stdio.h>
#include<stdlib.h>
#define SUCCESS 0
#define FAILURE -1
#define NOELEMENT -2
#define DUPLICATE -3
#define RED 1
#define BLACK 0


typedef struct node
{
	int data;
	struct node* parent;
	struct node *left;
	struct node *right;
	int color;
}node;

typedef struct redblack{
	node* root;
}REDBLACKTREE;



/* keep all the prototypes of the functions here */
node* insert(REDBLACKTREE* tree, int data);
node* insertfixup(REDBLACKTREE* tree,node* curr);
node* createnode(int data);
void rotate_left(REDBLACKTREE* tree,node* node);
void rotate_right(REDBLACKTREE* tree,node* node);
void preorder(node* root);
void inorder(node* root);
void postorder(node* root);
int search(node* root,int data);
int find_maximum(node *root, int *max);
int find_minimum(node *root, int *min);
int delete_minimum(REDBLACKTREE* tree);
int delete_maximum(REDBLACKTREE* tree);
node* delete(REDBLACKTREE* tree, int item);
void delete_fixup(REDBLACKTREE* tree,node* child);
void transplant(REDBLACKTREE* tree, node* n1,node*n2);
void print_tree_structured(node* root, int space, char child_type);
void print_tree(REDBLACKTREE* tree);

#endif
