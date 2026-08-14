/*******************************************************************************************************************************************************************
*Title			: Insertion
*Description		: This function performs inserting the new data into the given Red Black tree.
*Prototype		: int insert(tree_t **root, data_t item);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – New data to be inserted into the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

void left_rotate(tree_t **root,tree_t *parent, tree_t *x)
{
	tree_t *y = x->right;
	x->right = y->left;
	y->left = x;
	if(parent == NULL)
		*root = y;
	else if(parent->left == x)
		parent->left = y;
	else
		parent->right = y;
	
}
void right_rotate(tree_t **root,tree_t *parent, tree_t *y)
{
	tree_t *x = y->left;
	y->left = x->right;
	x->right = y;
	if(parent == NULL)
		*root = x;
	else if(parent->left == y)
		parent->left = x;
	else
		parent->right = x;
	
}
tree_t *find_parent(tree_t *root, tree_t *node)
{
	tree_t *current = root;
	tree_t *parent = NULL;
	while (current != NULL && current != node)
	{
		parent = current;
		if (node->data < current->data)
			current = current->left;
		else
			current = current->right;
	}
	return parent;
}
int insert(tree_t **root, data_t item)
{
	/* Definition goes here */
    //if(the tree is empty create a new node and assign it to root)with color black
	// if the tree is not empty, create a new node with color red and insert it in the appropriate position
	// if parent of the new node is black than exit 
	// if parent of the new node is red, then check the colour of the uncle node
	   // if the uncle node is red, then recolor the parent and uncle to black , check if parent of the parent of new node is not root node then recolour & recheck 

	   // if the uncle node is black, then perform rotation and recolor the nodes accordingly
	   if(*root == NULL)
	  {
		tree_t *new_node = (tree_t *)malloc(sizeof(tree_t));
		new_node->data = item;
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->color = 0; // Black
		*root = new_node;
		return SUCCESS;
	  }
	  else
	  {
		tree_t *new_node = (tree_t *)malloc(sizeof(tree_t));
		new_node->data = item;
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->color = 1; // Red
		// Insert the new node in the appropriate position
		tree_t *current = *root;
		tree_t *parent = NULL;
		while (current != NULL)
		{
			parent = current;
			if (item < current->data)
				current = current->left;
			else
				current = current->right;	
		}
		if (item < parent->data)
			parent->left = new_node;
		else
			parent->right = new_node;
		if(parent->color == 0) // Parent is black
			return SUCCESS;
		else{
            tree_t *node = new_node;
				int done = 0;

				while (!done)
				{
					tree_t *parent = find_parent(*root, node);
					if (parent == NULL || parent->color == 0)
					{
						done = 1;
						continue;
					}
					tree_t *grandparent = find_parent(*root, parent);
					if (grandparent == NULL)
					{
						done = 1;
						continue;
					}
					tree_t *uncle = (grandparent->left == parent) ? grandparent->right : grandparent->left;
					if (uncle != NULL && uncle->color == 1) // Uncle is red
					{
						parent->color = 0;
						uncle->color = 0;
						grandparent->color = 1;
						node = grandparent; 
					}
					else // Uncle is black or NULL
					{
						if (grandparent->left == parent)
						{
							if (parent->left == node) // Left Left Case
							{
								right_rotate(root, find_parent(*root, grandparent), grandparent);
								grandparent->color = 1;
								parent->color = 0;
							}
							else // Left Right Case
							{
								left_rotate(root, grandparent, parent);
								right_rotate(root, find_parent(*root, grandparent), grandparent);
								grandparent->color = 1;
								node->color = 0;
							}
						}
						else
						{
							if (parent->right == node) // Right Right Case
							{
								left_rotate(root, find_parent(*root, grandparent), grandparent);
								grandparent->color = 1;
								parent->color = 0;
							}
							else // Right Left Case
							{
								right_rotate(root, grandparent, parent);
								left_rotate(root, find_parent(*root, grandparent), grandparent);
								grandparent->color = 1;
								node->color = 0;
							}
						}
						done = 1;
					}
				}
				(*root)->color = 0;
				return SUCCESS;

		}
	  }


}
