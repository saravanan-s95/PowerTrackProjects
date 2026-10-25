#include "rbt.h"

void ll_rotation(tree_t** root,tree_t* greatgrandparent,tree_t* grandparent,tree_t* parent)
{
    /* Grandparent is root */
    if(grandparent == *root)
    {
        /* Change root to parent and connect grandparent */
        *root = parent;
        grandparent->left = parent->right;
        (*root)->right = grandparent;
    }    
    else
    {
        /* Connect greatgrandparent with parent */
        if(greatgrandparent->left == grandparent)
        {
            greatgrandparent->left = parent;
        }
        else
        {
            greatgrandparent->right = parent;
        }
        /* Connect parents right node to grandparents left */
        grandparent->left = parent->right;
        parent->right = grandparent;
    }
}

void rr_rotation(tree_t** root,tree_t* greatgrandparent,tree_t* grandparent,tree_t* parent)
{
    /* Grandparent is root */
    if(grandparent == *root)
    {
        /* Change root to parent and connect grandparent */
        *root = parent;
        grandparent->right = parent->left;
        (*root)->left = grandparent;
    }
    else
    {
        /* Connect greatgrandparent with parent */
        if(greatgrandparent->left == grandparent)
        {
            greatgrandparent->left = parent;
        }
        else
        {
            greatgrandparent->right = parent;
        }
        /* Connect parents right node to grandparents right */
        grandparent->right = parent->left;
        parent->left = grandparent;
    }
}