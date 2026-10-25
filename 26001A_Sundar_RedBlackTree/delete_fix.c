#include "rbt.h"

int delete_fix(tree_t** root,tree_t* parent,tree_t* sibling)
{
    /* Take temporary pointer to traverse the red black tree */
    tree_t* temp = *root;
    tree_t* grandparent = NULL;

    /* Find grandparent node */
    while(1)
    {
        if(parent->data < temp->data)
        {
            grandparent = temp;
            temp = temp->left;
        }
        else if(parent->data > temp->data)
        {
            grandparent = temp;
            temp = temp->right;
        }
        else
        {
            break;
        }
    }

    /* Sibling doesn't exist */
    if(sibling == NULL)
    {
        /* Parent color is black */
        if(parent->color == BLACK)
        {
            /* Parent is root */
            if(parent == *root)
            {
                return SUCCESS;
            }

            /* Parent is left of grandparent */
            if(grandparent->left == parent)
            {
                /* Recursively call delete_fix function */
                delete_fix(root,grandparent,grandparent->right);
            }
            /* Parent is right of grandparent */
            else
            {
                /* Recursively call delete_fix function */
                delete_fix(root,grandparent,grandparent->left);
            }
        }
        /* Parent color is red */
        else
        {
            /* Recolor parent to black and return success */
            parent->color = BLACK;
            return SUCCESS;
        }
    }

    /* Sibling exists and its both children are either black or null */
    else if(sibling->color == BLACK && (sibling->left == NULL || sibling->left->color == BLACK) && 
    (sibling->right == NULL || sibling->right->color == BLACK))
    {
        /* Recolor sibling to red */
        sibling->color = RED;

        /* Parent color is black */
        if(parent->color == BLACK)
        {
            /* Parent is root */
            if(parent == *root)
            {
                return SUCCESS;
            }

            /* Parent is left of grandparent */
            if(grandparent->left == parent)
            {
                /* Recursively call delete_fix function */
                delete_fix(root,grandparent,grandparent->right);
            }
            /* Parent is right of grandparent */
            else
            {
                /* Recursively call delete_fix function */
                delete_fix(root,grandparent,grandparent->left);
            }
        }
        /* Parent color is red */
        else
        {
            /* Recolor parent to black */
            parent->color = BLACK;
            return SUCCESS;
        }
    }

    /* Sibling exists and its color is red */
    else if(sibling->color == RED)
    {
        /* Recolor parent to red and sibling to black */
        parent->color = RED;
        sibling->color = BLACK;

        /* Sibling is left of parent */
        if(parent->left == sibling)
        {
            /* Do LL rotation */
            ll_rotation(root,grandparent,parent,sibling);
            /* Recursively call delete_fix function */
            delete_fix(root,parent,parent->left);
        }
        /* Sibling is right of parent */
        else
        {
            /* Do RR rotation */
            rr_rotation(root,grandparent,parent,sibling);
            /* Recursively call delete_fix function */
            delete_fix(root,parent,parent->right);
        }
    }

    /* Sibling exists and its color is black and is right of parent and near child exists and near child's color is red and far child is either null or exists and black in color */
    else if(sibling->color == BLACK && parent->right == sibling && (sibling->left && sibling->left->color == RED) && (sibling->right == NULL || sibling->right->color == BLACK))
    {
        /* Recolor sibling to red and near child to black */
        sibling->color = RED;
        sibling->left->color = BLACK;

        /* Do LL rotation */
        ll_rotation(root,parent,sibling,sibling->left);
        /* Recursively call delete_fix function */
        delete_fix(root,parent,parent->right);
    }

    /* Sibling exists and its color is black and is left of parent and near child exists and near child's color is red and far child is either null or exists and black in color */
    else if(sibling->color == BLACK && parent->left == sibling && (sibling->right && sibling->right->color == RED) && (sibling->left == NULL || sibling->left->color == BLACK))
    {
        /* Recolor sibling to red and near child to black */
        sibling->color = RED;
        sibling->right->color = BLACK;

        /* Do RR rotation */
        rr_rotation(root,parent,sibling,sibling->right);
        /* Recursively call delete_fix function */
        delete_fix(root,parent,parent->left);
    }

    /* Sibling exists and its color is black and is right of parent and far child exists and far child's color is red */
    else if(sibling->color == BLACK && parent->right == sibling && sibling->right && sibling->right->color == RED)
    {
        /* Swap sibling and parent colors */
        sibling->color = parent->color;
        parent->color = BLACK;

        /* Do RR rotation */
        rr_rotation(root,grandparent,parent,sibling);
        /* Recolor far child to black */
        sibling->right->color = BLACK;
    }

    /* Sibling exists and its color is black and is left of parent and far child exists and far child's color is red */
    else if(sibling->color == BLACK && parent->left == sibling && sibling->left && sibling->left->color == RED)
    {
        /* Swap sibling and parent colors */
        sibling->color = parent->color;
        parent->color = BLACK;

        /* Do LL rotation */
        ll_rotation(root,grandparent,parent,sibling);
        /* Recolor far child to black */
        sibling->left->color = BLACK;
    }
    return SUCCESS;
}