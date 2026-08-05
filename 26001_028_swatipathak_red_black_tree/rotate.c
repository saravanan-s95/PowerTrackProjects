#include "rbt.h"

void rotate_left(REDBLACKTREE* tree,node* x){
    node* newnode=x->right;
    x->right=newnode->left;
    if (newnode->left != NULL) {
        newnode->left->parent = x;
    }
    newnode->parent=x->parent;
    if(x->parent==NULL){
        tree->root=newnode;
    }
    else if(x==x->parent->right){
        x->parent->right=newnode;
    }
    else{
        x->parent->left=newnode;
    }
    newnode->left=x;
    x->parent=newnode;
}
void rotate_right(REDBLACKTREE* tree,node* x){
    node* newnode=x->left;
    x->left=newnode->right;
    if (newnode->right != NULL) {
        newnode->right->parent = x;
    }
    newnode->parent=x->parent;
    if(x->parent==NULL){
        tree->root=newnode;
    }
    else if(x=x->parent->left){
        x->parent->left=newnode;
    }
    else{
        x->parent->right=newnode;
    }
    newnode->right=x;
    x->parent=newnode;
}