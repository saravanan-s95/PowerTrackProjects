/*******************************************************************************************************************************************************************
*Title			: Deletion
*Description		: This function performs deleting of the given data from the given Red Black tree.
*Prototype		: int delete(tree_t **root, data_t item); 
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – Data to be deleted from the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete(tree_t **root, data_t item)
{
	/* Definition goes here */

    /* Tree is empty */
	if(*root == NULL)
    {
        printf("Tree is empty\n");
        return FAILURE;
    }

	tree_t* node = *root;
    tree_t* parent = NULL;

    /* Find parent node and the child node to be deleted */
    while(node)
    {
        /* Value is less than node value */
        if(item < node->data)
        {
            /* Update parent node and move left */
            parent = node;
            node = node->left;
        }
        /* Value is more than node value */
        else if(item > node->data)
        {
            /* Update parent node and move right */
            parent = node;
            node = node->right;
        }
        else
        {
            /* Value is equal to node value, so break */
            break;
        }
    }
    
    /* Node to be deleted doesn't exist */
    if(node == NULL)
    {
        printf("Data not found\n");
        return FAILURE;
    }

    /* Leaf node condition */
    if(node->left == NULL && node->right == NULL)
    {
        /* Node is root */
        if(node == *root)
        {
            /* Delete node and make tree null and return */
            free(node);
            *root = NULL;
            return SUCCESS;
        }

        /* Node is red color */
        if(node->color == RED)
        {
            /* Make the parent node direction left or right null based on the node placement */
            if(parent->left == node)
            {
                parent->left = NULL;
            }
            else if(parent->right == node)
            {
                parent->right = NULL;
            }
            /* Delete the node and return */
            free(node);
            return SUCCESS;
        }
        /* Node is black color */
        else
        {
            /* Make the parent node direction left or right null based on the node placement */
            if(parent->left == node)
            {
                /* Handle double black cases */
                parent->left = NULL;
                delete_fix(root,parent,parent->right);
            }
            else if(parent->right == node)
            {
                /* Handle double black cases */
                parent->right = NULL;
                delete_fix(root,parent,parent->left);
            }
            /* Delete the node */
            free(node);
        }
    }

    /* Left child node doesn't exist and right child node exists */
    else if(node->left == NULL && node->right != NULL)
    {
        /* Copy right child node data to current node */
        tree_t* temp = node->right;
        node->data = temp->data;
        node->left = temp->left;
        node->right = temp->right;

        /* Right child node color is red */
        if(temp->color == RED)
        {
            /* Delete node and return */
            free(temp);
            return SUCCESS;
        }
        /* Right child node color is black */
        else
        {
            /* Delete node and handle double black cases */
            free(temp);
            delete_fix(root,node,NULL);
        }
    }

    /* Right child node doesn't exist and left child node exists */
    else if(node->right == NULL && node->left != NULL)
    {
        /* Copy left child node data to current node */
        tree_t* temp = node->left;
        node->data = temp->data;
        node->left = temp->left;
        node->right = temp->right;

        /* Left child node color is red */
        if(temp->color == RED)
        {
            /* Delete node and return */
            free(temp);
            return SUCCESS;
        }
        /* Left child node color is black */
        else
        {
            /* Delete node and handle double black cases */
            free(temp);
            delete_fix(root,node,NULL);
        }
    }

    /* Both left and right child nodes exist */
    else
    {
        /* Find minimum node value to the right tree structure of the current node */
        int min;
        find_minimum(&node->right,&min);

        tree_t* min_node = *root;
        tree_t* min_node_parent = NULL;

        /* Find minimum node and its parent */
        while(min_node->data != min)
        {
            if(min < min_node->data)
            {
                min_node_parent = min_node;
                min_node = min_node->left;
            }
            else
            {
                min_node_parent = min_node;
                min_node = min_node->right;
            }
        }

        /* Find whether min node exists to the right or left of min node's parent */
        int is_left = (min_node_parent->left == min_node);

        /* Left */
        if(is_left)
        {
            /* Min node right value is connected to min node parent's left */
            min_node_parent->left = min_node->right;
        }
        /* Right */
        else
        {
            /* Min node right value is connected to min node parent's left */
            min_node_parent->right = min_node->right;
        }

        /* Copy only the value of minimum node into node to be deleted */
        node->data = min;
        
        /* Min node color is red */
        if(min_node->color == RED)
        {
            /* Delete node and return success */
            free(min_node);
            return SUCCESS;
        }
        /* Min node color is black */
        else
        {
            /* Check whether the node that replaced min node exists */
            tree_t* x = is_left ? min_node_parent->left : min_node_parent->right;

            /* Delete min node */
            free(min_node);

            /* x node exists and its color is red */
            if(x != NULL && x->color == RED)
            {
                /* Recolor x node to black and return success */
                x->color = BLACK;
                return SUCCESS;
            }

            /* x node doesn't exist and min node was to the left of the parent */
            if(is_left)
            {
                /* Handle double black cases by passing parent and sibling of the current node */
                delete_fix(root,min_node_parent,min_node_parent->right);
            }
            /* x node doesn't exist and min node was to the right of the parent */
            else
            {
                /* Handle double black cases by passing parent and sibling of the current node */
                delete_fix(root,min_node_parent,min_node_parent->left);
            }
        }
    }
    return SUCCESS;
}