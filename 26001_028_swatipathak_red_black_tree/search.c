#include "rbt.h"

int search(node* root,int data){
    if(root==NULL)
        return NOELEMENT;

    while(root!=NULL){
        if(root->data > data)
            root=root->left;
        else if(root->data < data)
            root=root->right;
        else{
            return SUCCESS;
        }
    }
    return NOELEMENT;
}
