/*******************************************************************************************************************************************************************
*Title			: Deletion
*Description		: This function performs deleting of the given data from the given Red Black tree.
*Prototype		: int delete(tree_t **root, data_t item); 
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – Data to be deleted from the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"
extern int status;

void delete_fixup(REDBLACKTREE* tree,node* child){
	if(child==NULL && tree->root==NULL)
		return;
	while(child!=tree->root && (child==NULL || child->color==BLACK)){
		node* parent=child?child->parent:NULL;
		if(parent==NULL) break;
		
		if(child==parent->left){
			node* sibling=parent->right;
			if(sibling && sibling->color==RED){
				parent->color=RED;
				sibling->color=BLACK;
				rotate_left(tree,parent);
				sibling=parent->right;
			}
			if(sibling==NULL ||((sibling->left==NULL||sibling->left->color==BLACK)&&(sibling->right==NULL || sibling->right->color==BLACK))){
				if(sibling)
					sibling->color=RED;
					child=parent;
			}
			else if(sibling&&(sibling->right==NULL || sibling->right->color==BLACK)){
				if(sibling->left)
					sibling->left->color=BLACK;
				sibling->color=RED;
				rotate_right(tree,sibling);
				sibling=parent->right;
				sibling->color=parent->color;
				parent->color=BLACK;
				if(sibling->right)sibling->right->color=BLACK;
				rotate_left(tree,parent);
				child=tree->root;
			}
			else{
				if(sibling){
					sibling->color=parent->color;
					if(sibling->right)sibling->right->color=BLACK;
				}
				parent->color=BLACK;
				rotate_left(tree,parent);
				child=tree->root;
			}
		}
		else{
			node* sibling=parent->left;

			if(sibling && sibling->color==RED){
				sibling->color=BLACK;
				parent->color=RED;
				rotate_right(tree,parent);
				sibling-parent->left;
			}
			if(sibling==NULL ||((sibling->left==NULL||sibling->left->color==BLACK)&&(sibling->right==NULL || sibling->right->color==BLACK))){
				if(sibling)
					sibling->color=RED;
					child=parent;
			}
			else if(sibling&&(sibling->left==NULL || sibling->left->color==BLACK)){
				if(sibling->right)
					sibling->right->color=BLACK;
				sibling->color=RED;
				rotate_left(tree,sibling);
				sibling=parent->left;
				sibling->color=parent->color;
				parent->color=BLACK;
				if(sibling->left)sibling->left->color=BLACK;
				rotate_right(tree,parent);
				child=tree->root;
			}
			else{
				if(sibling){
					sibling->color=parent->color;
					if(sibling->left)sibling->left->color=BLACK;
				}
				parent->color=BLACK;
				rotate_right(tree,parent);
				child=tree->root;
			}
		}
	}
}
void transplant(REDBLACKTREE* tree, node* n1,node*n2){
	if(n1->parent==NULL){
		tree->root=n2;
	}
	else if(n1->parent->left==n1){
		n1->parent->left=n2;
	}
	else{
		n1->parent->right=n2;
	}
	if(n2!=NULL)
		n2->parent=n1->parent;
}
node* delete(REDBLACKTREE* tree, int data)
{
	node* delnode=tree->root;
	while(delnode!=NULL && delnode->data!=data){
		if(data> delnode->data)
			delnode=delnode->right;
		else	
			delnode=delnode->left;
	}
	if(delnode==NULL){
		status=FAILURE;
		return tree->root;
	}
	node* child;
	node* curr=delnode;
	int original_col=curr->color;

	if(delnode->left==NULL){
		child=delnode->right;
		transplant(tree,delnode, delnode->right);
	}
	else if(delnode->right==NULL){
		child=delnode->left;
		transplant(tree,delnode, delnode->left);
	}
	else{
		curr=delnode->right;
		while(curr->left!=NULL){
			curr=curr->left;
		}
		original_col=curr->color;
		child=curr->right;
		if(curr->parent!=delnode){
			transplant(tree,curr,curr->right);
			curr->right=delnode->right;
			if(curr->right!=NULL)
				curr->right->parent=curr;
		}
		transplant(tree,delnode,curr);
		curr->left=delnode->left;
		if(delnode->left!=NULL){
			delnode->left->parent=curr;
		}
		curr->color=delnode->color;
	}
	free(delnode);
	if(original_col==BLACK){
		delete_fixup(tree,child);
	}
	status=SUCCESS;
	return tree->root;
}
