#include<stdio.h>
#include<stdlib.h>
#include "red_black_tree.h"

tree_t *search(tree_t *root, data_t item)
{
    while(root != NIL && root != NULL)
    {
        if(item == root->data)   //item found at current node
        {
            return root;
        }
        else if(item < root->data)   // is smaller search in a left side
        {
            root = root->left;
        }
        else
        {
            root = root->right;     //is larger search in right side
        }

    }
    return NIL;
}

int find_maximum(tree_t **root, data_t *max)
{
    if(*root == NIL)        //tree is empty
        return FAILURE;

    tree_t *temp = *root;    //take a temp variable
    while(temp->right != NIL)
        temp = temp->right;    // move to next right node

    *max = temp->data;         //store the max value
    return SUCCESS;
}

int find_minimum(tree_t **root, data_t *min)
{
    if(*root == NIL)           //tree is empty
        return FAILURE;

    tree_t *temp = *root;      //take a temp variable
    while(temp->left != NIL)
        temp = temp->left;     //move to next left node

    *min = temp->data;         //store the min value
    return SUCCESS;
}

tree_t *parent(tree_t *node)
{
    if(node == NIL)
    {
        return NIL;
    }
    return node->parent;      //return parent pointer
}


tree_t *siblings(tree_t *node)
{
    if(node == NIL || node->parent == NIL)
    {
        return NIL;
    }
    if(node == node->parent->left)      // node is left child
        return node->parent->right;     // siblings in right child
    else
        return node->parent->left;      // siblings in left child
}

void transplant(tree_t **root, tree_t *u, tree_t *v)
{
    if(u->parent == NIL)
        *root = v;                      //take v as root
    else if(u == u->parent->left)
        u->parent->left = v;            // v as left child
    else
        u->parent->right = v;           // v as right child

    v->parent = u->parent;             //update v to u parent
}

void delete_fix(tree_t **root, tree_t *x)
{
    while (x != *root && x->colour == BLACK)
    {
        if (x == x->parent->left)           // x is left child of his parent
        {
            tree_t *w = x->parent->right;  // w is siblings of x

            if (w->colour == RED) 
            {
                w->colour = BLACK;
                x->parent->colour = RED;   //parent RED
                left_rotate(root, x->parent); 
                w = x->parent->right;      // update siblings
            }
            if (w->left->colour == BLACK && w->right->colour == BLACK)   //siblings and both child are black
            {
                w->colour = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->colour == BLACK) 
                {
                    w->left->colour = BLACK;     // siblings left is black
                    w->colour = RED;
                    right_rotate(root, w);       // right rotate
                    w = x->parent->right;        // update
                }
                
                w->colour = x->parent->colour;    
                x->parent->colour = BLACK;
                w->right->colour = BLACK;       // right siblings are black
                left_rotate(root, x->parent);   //left rotate parent
                x = *root;
            }
        }
        else 
        {
            tree_t *w = x->parent->left;    

            if (w->colour == RED)
            {
                w->colour = BLACK;         // siblings black
                x->parent->colour = RED;   //parent RED
                right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->colour == BLACK && w->left->colour == BLACK) // siblings and their child is black
            {
                w->colour = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->colour == BLACK)   // siblings left child is black
                {
                    w->right->colour = BLACK;
                    w->colour = RED;
                    left_rotate(root, w);
                    w = siblings(x);
                }
                w->colour = x->parent->colour;     // copy parent colour to siblings
                x->parent->colour = BLACK;
                w->left->colour = BLACK;
                right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->colour = BLACK;
}

tree_t *tree_minimum(tree_t *root)
{
    if(root == NIL)
        return NIL;

    while(root->left != NIL)
    {
        root = root->left;         //move to next left
    }
    return root;    
}

tree_t *tree_maximum(tree_t *root)
{
    if (root == NIL)
        return NIL;

    while (root->right != NIL)
    {
        root = root->right;           //move to next right
    }
    return root;
}


    
int delete(tree_t **root, data_t item)
{
    tree_t *z = search(*root, item);      //search the deleted node
    if (z == NIL)
        return FAILURE;

    tree_t *y = z;                         
    int y_original_colour = y->colour;
    tree_t *x;                             // x is child 

    if (z->left == NIL)                   // z has no left child
    {
        x = z->right;
        transplant(root, z, z->right);    //replace z with right child
    }
    else if (z->right == NIL)             // z has no right child
    {
        x = z->left;
        transplant(root, z, z->left);     // replace z with left child
    }
    else
    {
        y = tree_minimum(z->right);
        y_original_colour = y->colour;    // save the successor colour
        x = y->right;

        if (y->parent == z)               //successor parent is z ,x parent should be y
        {
            x->parent = y;
        }
        else
        {
            transplant(root, y, y->right);  // y with his right child
            y->right = z->right;            // y subtree as z subtree
            y->right->parent = y;
        }

        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->colour = z->colour;          // keep original colour of z
    }

    free(z);

    if (y_original_colour == BLACK)   
        delete_fix(root, x);

    return SUCCESS;
   
}

int delete_minimum(tree_t **root)
{
    if (*root == NIL) 
        return FAILURE;
    tree_t *z = tree_minimum(*root);    // find min node
    tree_t *x = z->right;               // x will replace with z ,only has right child
    tree_t *y = z;                       //deleted
    int y_col = y->colour;               //save y original colour
 
    transplant(root, z, z->right);       // replace z with right child
    free(z);                             // delete z

    if (y_col == BLACK)                  // violation happens          
    delete_fix(root, x);                  // fix it              
    return SUCCESS;
}

int delete_maximum(tree_t **root)
{
    if (*root == NIL) 
        return FAILURE;
    tree_t *z = tree_maximum(*root);  // find max node
    tree_t *x = z->left;              // x will replace with z ,only has left child
    tree_t *y = z;                    //deleted
    int y_col = y->colour;            //save y original colour

    transplant(root, z, z->left);     // replace z with left child
    free(z);                          // delete z
    
    if (y_col == BLACK)               // violation happens
    delete_fix(root, x);              // fix it
    return SUCCESS;
}