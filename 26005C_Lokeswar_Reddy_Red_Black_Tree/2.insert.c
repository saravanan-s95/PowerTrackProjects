/*
Insertion Rules:
1. If tree is empty, create new node as root with color BLACK
2. If tree is not empty, create newnode as leaf node with color RED
3. If parent of new node is BLACk, then exit
4. If parent of new node is RED, then check color of parent's sibling of newnode
	4.1. If color is BLACK or node is NULL, then do suitable rotations and recolor
	4.2. if color is RED, then recolor both parent node and parent's sibling node.
	     check if parent's parent of newnode, if it is not root node then recolor and recheck
*/

#include "header.h"
#include <stdlib.h>

static void check_after_insert(tree_t **root, tree_t *node);

int insert(tree_t **root, data_t data)
{
	tree_t *new=(tree_t *)malloc(sizeof(tree_t));
	if(new==NULL)
	{
		return FAILURE;
	}
	new->data = data;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	new->color = RED;

	tree_t *parent=NULL;
	tree_t *temp = *root;

	while(temp!=NULL)
	{
		parent=temp;
		if(data < temp->data)
		{
			temp=temp->left;
		}
		else if(data > temp->data)
		{
			temp = temp->right;
		}
		else
		{
			free(new);
			return FAILURE;
		}
	}
    new->parent = parent;

	if(parent == NULL)
	   *root = new;
	else if(data < parent->data)
		parent->left = new;
	else
		parent->right =new;
	  
	check_after_insert(root,new);
	return SUCCESS;
}


static void check_after_insert(tree_t **root, tree_t *node)
{
	tree_t *current=node;
	tree_t *prev;
	tree_t *temp;

	while(current != *root && current->parent!=NULL && current->parent->color == RED)
	{
		prev=current->parent;
		if(prev == prev->parent->left)
		{
			temp=prev->parent->right;
			//checks parent sibling is whether RED or not
			if(temp != NULL && temp->color == RED)
			{
				prev->color=BLACK;
				temp->color=BLACK;
			    prev->parent->color=RED;
				current=prev->parent;
			}
			else
			{
				// left rotation and then right rotation
				if(current == prev->right)
				{
					current=prev;
					left_rotate(root,current);
					prev=current->parent;
				}
				prev->color=BLACK;
				prev->parent->color=RED;
				right_rotate(root,prev->parent);
			}
		}
		else
		{
			temp=prev->parent->left;
			//checks parent sibling is whether RED or not
			if(temp != NULL && temp->color == RED)
			{
				prev->color=BLACK;
				temp->color=BLACK;
				prev->parent->color=RED;
				current=prev->parent;
			}
			else
			{
				// right rotation and then left rotation
				if(current == prev->left)
				{
					current=prev;
					right_rotate(root,current);
					prev=current->parent;
				}
				prev->color=BLACK;
				prev->parent->color=RED;
				left_rotate(root,prev->parent);
			}
		}
	}
	if(*root !=NULL)
	{
		(*root)->color=BLACK;
		(*root)->parent=NULL;
	}
}