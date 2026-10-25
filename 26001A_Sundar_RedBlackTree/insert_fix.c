#include "rbt.h"

void fix_violation(tree_t** root,tree_t* node)
{
    tree_t* greatgrandparent = NULL;
    tree_t* grandparent = NULL;
    tree_t* parent = *root;

    /* Get greatgrandparent and grandparent and parent nodes */
    while(1)
    {
        if(node->data < parent->data)
        {
            if(parent->left == node)
            {
                break;
            }

            greatgrandparent = grandparent;
            grandparent = parent;
            parent = parent->left;
        }
        else
        {
            if(parent->right == node)
            {
                break;
            }

            greatgrandparent = grandparent;
            grandparent = parent;
            parent = parent->right;
        }
    }

    /* Parent is root or its color is black return */
    if(parent == *root || parent->color == BLACK)
    {
        return;
    }

    /* Parent node is left to grandparent */
    if(grandparent->left == parent)
	{
        /* Uncle node exists and its color is red */
		if(grandparent->right && grandparent->right->color == RED)
		{
            /* Recolor both uncle and parent nodes to black */
			grandparent->right->color = BLACK;
			parent->color = BLACK;
		
            /* Grandparent node is root */
		    if(grandparent == *root)
		    {
                return;   
		    }

            /* Recolor grandparent node if grandparent is not root node */
            grandparent->color = RED;
            fix_violation(root,grandparent);
        }
        /* LL rotation */
        else if(parent->left == node)
        {
            /* Recolor parent and grandparent */ 
            parent->color = BLACK;
            grandparent->color = RED;

            ll_rotation(root,greatgrandparent,grandparent,parent);
        }
        /* LR rotation */
        else
        {
            /* Swap parent and child node or shift left */
            grandparent->left = node;
            parent->right = node->left;   
            node->left = parent;

            parent = node;

            /* Recolor parent and grandparent */ 
            parent->color = BLACK;
            grandparent->color = RED;

            /* Do LL rotation */
            ll_rotation(root,greatgrandparent,grandparent,parent);
        }
	}
    /* Parent node is right to grandparent */
	else
	{
        /* Uncle node exists and its color is red */
		if(grandparent->left && grandparent->left->color == RED)
		{
            /* Recolor both uncle and parent nodes to black */
			grandparent->left->color = BLACK;
			parent->color = BLACK;

            /* Grandparent node is root */
		    if(grandparent == *root)
		    {
			    return;
		    }

            /* Recolor grandparent node if grandparent is not root node */
            grandparent->color = RED;
            fix_violation(root,grandparent);
        }
        /* RR rotation */
        else if(parent->right == node)
        {
            /* Recolor parent and grandparent */ 
            parent->color = BLACK;
            grandparent->color = RED;

            rr_rotation(root,greatgrandparent,grandparent,parent);
        }
        /* RL rotation */
        else
        {
            /* Swap parent and child or shift right */
            grandparent->right = node;
            parent->left = node->right;
            node->right = parent;

            parent = node;

            /* Recolor parent and grandparent */
            parent->color = BLACK;
            grandparent->color = RED;

            /* Do RR rotation */
            rr_rotation(root,greatgrandparent,grandparent,parent);
        }
	}
}