#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//pre-order
void print_tree(tree_t *root)
{
    if(root == NULL)
        return;

    if(root != NULL)
    {
        if(root->color == RED)
            printf("\033[31m(%d)(R)\033[0m-->",root->data);
        else
            printf("(%d)(B)-->",root->data);

        print_tree(root->left);
        
        print_tree(root->right);
    }
}