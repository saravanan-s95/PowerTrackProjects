/*
Deletion Rules:
step 1 : perform BST Deletion
step 2 : 
	case 1: If node to be deleted is RED, just delete it
	case 2: If root is DOUBLE BLACk(DB), just replace DB with BLACK
	case 3: If DB's sibling is BLACK and both its child are also black
		=> Then remove DB
		=> Add BLACK to the parent of DB node
			=> If parent is RED, it will become BLACK
			=> If parent is BLACK, it will become DOUBLE BLACK(DB)
		=> Recolor sibling of original DB with RED
		=> If DB still exist, choose relevent case and proceed
		[ If we are deleting Leaf node which is in BLACK, then it will become DB NIL node]
	case 4: If DB's sibling is RED
		=> swap colors of DB's (parent and sibling) nodes
		=> rotate parent of DB, in DB's direction
		=> recheck suitable case
	case 5: DB's sibling is BLACK and sibling's child who is far from DB is BLACK, but near child is RED
		=> swap colors of DB's sibling & sibling's child who is near to DB
		=> rotate sibling in opposite direction to DB
		=> Recheck cases
	case 6: DB's sibling is BLACK and far child is RED
		=> swap color of parent & sibling of DB
		=> rotate DB's parent in DB's direction
		=> remove DB
		=> change color of RED child to BLACK
*/

#include "header.h"
#include <stdlib.h>

static tree_t *find_node(tree_t *root, data_t data);
static void replace_node(tree_t **root, tree_t *old_node, tree_t *new_node);

static void check_after_delete(tree_t **root, tree_t *node, tree_t *parent);

static tree_t *min_node(tree_t *node);
static int node_color(tree_t *node);



int delete(tree_t **root, data_t data)
{
	tree_t *node;
	tree_t *successor;
	tree_t *replacement;
	tree_t *replacement_parent;
	int removed_color;

	if (root == NULL || *root == NULL)
		return FAILURE;

	node = find_node(*root, data); 
	if (node == NULL)
		return FAILURE;

	successor = node;
	removed_color = successor->color;

	if (node->left == NULL)
	{
		replacement = node->right;
		replacement_parent = node->parent;
		replace_node(root, node, replacement);
	}
	else if (node->right == NULL)
	{
		replacement = node->left;
		replacement_parent = node->parent;
		replace_node(root, node, replacement);
	}
	else
	{
		successor = min_node(node->right);
		removed_color = successor->color;
		replacement = successor->right;

		if (successor->parent == node)
		{
			replacement_parent = successor;
			if (replacement != NULL)
				replacement->parent = successor;
		}
		else
		{
			replacement_parent = successor->parent;
			replace_node(root, successor, replacement);
			successor->right = node->right;
			successor->right->parent = successor;
		}

		replace_node(root, node, successor);
		successor->left = node->left;
		successor->left->parent = successor;
		successor->color = node->color;
	}

	free(node);

	if (removed_color == BLACK)
	{
		if (replacement != NULL)
			replacement_parent = replacement->parent;
		check_after_delete(root, replacement, replacement_parent);
	}
	return SUCCESS;
}

static tree_t *find_node(tree_t *root, data_t data)
{
	while (root != NULL)
	{
		if (data == root->data)
			return root; 
		else if (data < root->data)
			root = root->left;
		else
			root = root->right;
	}
	return NULL;
}

static void replace_node(tree_t **root, tree_t *old_node, tree_t *new_node)
{
	if (old_node->parent == NULL)
		*root = new_node;
	else if (old_node == old_node->parent->left)
		old_node->parent->left = new_node;
	else
		old_node->parent->right = new_node;

	if (new_node != NULL)
		new_node->parent = old_node->parent;
}

static tree_t *min_node(tree_t *node)
{
	if (node == NULL)
		return NULL;

	while (node->left != NULL)
		node = node->left;

	return node;
}

static int node_color(tree_t *node)
{
	return node == NULL ? BLACK : node->color;
}

static void check_after_delete(tree_t **root, tree_t *node, tree_t *parent)
{
	tree_t *sibling;

	while (node != *root && node_color(node) == BLACK)
	{
		if (parent == NULL)
			break;

		if (node == parent->left)
		{
			sibling = parent->right;

			if (node_color(sibling) == RED)
			{
				sibling->color = BLACK;
				parent->color = RED;
				left_rotate(root, parent);
				sibling = parent->right;
			}

			if (sibling == NULL)
			{
				node = parent;
				parent = node->parent;
			}
			else if (node_color(sibling->left) == BLACK &&
					node_color(sibling->right) == BLACK)
			{
				sibling->color = RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (node_color(sibling->right) == BLACK)
				{
					if (sibling->left != NULL)
						sibling->left->color = BLACK;
					sibling->color = RED;
					right_rotate(root, sibling);
					sibling = parent->right;
				}

				sibling->color = parent->color;
				parent->color = BLACK;
				if (sibling->right != NULL)
					sibling->right->color = BLACK;
				left_rotate(root, parent);
				node = *root;
				parent = NULL;
			}
		}
		else
		{
			sibling = parent->left;

			if (node_color(sibling) == RED)
			{
				sibling->color = BLACK;
				parent->color = RED;
				right_rotate(root, parent);
				sibling = parent->left;
			}

			if (sibling == NULL)
			{
				node = parent;
				parent = node->parent;
			}
			else if (node_color(sibling->left) == BLACK &&
					node_color(sibling->right) == BLACK)
			{
				sibling->color = RED;
				node = parent;
				parent = node->parent;
			}
			else
			{
				if (node_color(sibling->left) == BLACK)
				{
					if (sibling->right != NULL)
						sibling->right->color = BLACK;
					sibling->color = RED;
					left_rotate(root, sibling);
					sibling = parent->left;
				}

				sibling->color = parent->color;
				parent->color = BLACK;
				if (sibling->left != NULL)
					sibling->left->color = BLACK;
				right_rotate(root, parent);
				node = *root;
				parent = NULL;
			}
		}
	}
	if (node != NULL)
		node->color = BLACK;
}