#include <stdio.h>
#include<stdlib.h>
#include "red_black_tree.h"

tree_t *NIL;

tree_t * create_node(data_t item)               // create node
{
    tree_t *n = (tree_t *)malloc(sizeof(tree_t));  //create memory
    n->data = item;
    n->colour = RED;
    n->left = n->right = n->parent = NIL;         // initialized as NIL
    return n;

}


void init_tree(tree_t **root)               // initialization
{
    if(NIL == NULL)
    {
        NIL = (tree_t *)malloc(sizeof(tree_t));
        NIL->colour = BLACK;
        NIL->data = 0;
        NIL->left = NIL->right = NIL->parent = NIL;
    }
    *root = NIL;
}

int insertion(tree_t **root, data_t item)
{
    tree_t *t = create_node(item);
    tree_t *y = NIL;
    tree_t *x = *root;

    while(x != NIL)
    {
        y = x;                        // save as parent
        if(t->data < x->data)
            x = x->left;
        else if(t->data > x->data)
            x = x->right;
        else
        {
            free(t);
            return FAILURE; 
        }
    }
         
    
    t->parent = y;         
    if(y == NIL)
    {
        *root = t;
        t->colour = BLACK;            // root is always black
        return SUCCESS;
    }
    else if(t->data < y->data)
        y->left = t;
    else
        y->right = t;  
        
    printf("\nBefore balancing Red Black Tree is: \n");
    inorder_display(*root);   
    
    printf("\nNow Tree is balance\n");
    insertion_fix(root, t);
    
    inorder_display(*root);
    printf("\n");

    return SUCCESS;
}

void insertion_fix(tree_t **root, tree_t * z)  // helper function
{
    while(z->parent->colour == RED)             
    {
        if(z->parent == z->parent->parent->left)  // parent is left child of grandparent
        {
            tree_t *y = z->parent->parent->right;  

            if(y->colour == RED)
            {
                z->parent->colour = BLACK;
                y->colour = BLACK;
                z->parent->parent->colour = RED;  // grandfather as RED
                z = z->parent->parent;
            
            }
            else 
            {
                
                if(z == z->parent->right)
                {
                    z = z->parent;             //move z to parent
                    left_rotate(root, z);      // left rotation happen
                }
                z->parent->colour = BLACK;
                z->parent->parent->colour = RED;
                right_rotate(root, z->parent->parent);  // right rotate of grandparent
            }
        }
        else 
        {
            tree_t *y = z->parent->parent->left;   

            if(y->colour == RED) 
            {
                z->parent->colour = BLACK;
                y->colour = BLACK;
                z->parent->parent->colour = RED;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->left)  // z is left child
                {
                    z = z->parent;           
                    right_rotate(root, z);
                }
                z->parent->colour = BLACK;
                z->parent->parent->colour = RED;
                left_rotate(root, z->parent->parent);
            
            }
        }
    }
    (*root)->colour = BLACK;           // root must always be black
}


void left_rotate(tree_t **root, tree_t *x)
{
    tree_t *y = x->right;           //  y is right child of x
    x->right = y->left;

    if(y->left != NIL) 
        y->left->parent = x;        // update parent

    y->parent = x->parent;        // y to x parent

    if(x->parent == NIL) 
        *root = y; 
    else if(x == x->parent->left)   // x left child make y as left child
        x->parent->left = y;
    else 
        x->parent->right = y;      //x right child make y as right child

    y->left = x;
    x->parent = y;
}


void right_rotate(tree_t **root, tree_t *x)
{
    tree_t *y = x->left;            //  y is left child of x
    x->left = y->right;
    
    if(y->right != NIL)
        y->right->parent = x;      // update parent
    
    y->parent = x->parent;        // y to x parent
    
    if(x->parent == NIL)
        *root = y;                 // new root
    else if(x == x->parent->left) // x left child make y as left child
        x->parent->left = y;
    else
        x->parent->right = y;     //x right child make y as right child
    
    y->right = x;
    x->parent = y;
}

void inorder_display(tree_t *root)    // inoreder display
{
    
    if(root == NULL || root == NIL)
        return;
    inorder_display(root->left);
    if(root->colour == RED)
        printf("(%d)--(RED->%d)  ", root->data, RED);
    else
        printf("(%d)--(BLACK->%d)  ", root->data, BLACK);
    inorder_display(root->right);
}

    





