#include "rbt.h"

void search_data(tree_t* root,data_t item)
{
    /* Tree is empty */
    if(root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    /* Find the data by traversing the tree */
    while(root)
    {
        if(item < root->data)
        {
            root = root->left;
        }
        else if(item > root->data)
        {
            root = root->right;
        }
        else
        {
            break;
        }
    }

    /* Data not found */
    if(root == NULL)
    {
        printf("Data not found\n");
        return;
    }

    /* Data found so print the data */
    printf("Data found\n");
    printf("(%d)--(%s)\n",root->data,root->color == RED ? "RED->0" : "BLACK->1");
}