#include "rbt.h"

void inOrder(Tree *root)
{
    if (root == NULL)
        return;

    inOrder(root->left);

    printf("(%d) -- ", root->data);
    if (root->color == RED)
        printf("(%s->%d)\t", "RED", RED);
    else
        printf("(%s->%d)\t", "BLACK", BLACK);

    inOrder(root->right);
}

void display(Tree *root)
{
    inOrder(root);
    printf("\n");
}