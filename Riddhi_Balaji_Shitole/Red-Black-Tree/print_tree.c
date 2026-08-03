#include "rbt.h"

void print_tree(tree_t *root)
{
    static int space = 0;

    if(root == NULL)
        return;

    space += 10;

    print_tree(root->right);

    printf("\n");

    for(int i = 10; i < space; i++)
        printf(" ");

    printf("%d(%c)\n",
           root->data,
           root->color == RED ? 'R' : 'B');

    print_tree(root->left);

    space -= 10;
}