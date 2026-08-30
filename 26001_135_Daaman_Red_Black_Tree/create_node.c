#include "rbt.h"

Tree *create_node(int data)
{
    // Creating a node
    Tree *new = malloc(sizeof(Tree));
    if (new == NULL)
    {
        printf("ERROR: Tree Node could not be created\n");
        return NULL;
    }

    // Inserting data into node
    new->data = data;
    new->color = RED;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;

    return new;
}