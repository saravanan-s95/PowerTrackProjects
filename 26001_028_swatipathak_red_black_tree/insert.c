/*******************************************************************************************************************************************************************
*Title			: Insertion
*Description		: This function performs inserting the new data into the given Red Black tree.
*Prototype		: int insert(tree_t **root, data_t item);
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – New data to be inserted into the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"
extern int status;

node* insertfixup(REDBLACKTREE* tree,node* curr){
	while(curr->parent!=NULL && curr->parent->color==RED){
		node* uncle;
		 if(curr->parent==curr->parent->parent->right){
			uncle=curr->parent->parent->left;
			if(uncle && uncle->color==RED){
				uncle->color=BLACK;
				curr->parent->color=BLACK;
				curr->parent->parent->color=RED;
				curr=curr->parent->parent;
		 	}
			else{
				node* gp=curr->parent->parent;
				if(curr==curr->parent->left){
					curr=curr->parent;
					rotate_right(tree,curr);
				}
				curr->parent->color=BLACK;
				gp->color=RED;
				rotate_left(tree,gp);
			}
		 }
		 else{
			uncle=curr->parent->parent->right;
			if(uncle && uncle->color==RED){
				uncle->color=BLACK;
				curr->parent->color=BLACK;
				curr->parent->parent->color=RED;
				curr=curr->parent->parent;
			}
			else{
				node* gp=curr->parent->parent;
				if(curr==curr->parent->right){
					curr=curr->parent;
					rotate_left(tree,curr);
				}
				curr->parent->color=BLACK;
				gp->color=RED;
				rotate_right(tree,gp);
			}

		}
	}
	if(tree->root!=NULL)
		tree->root->color=BLACK;
	return curr;
}

node* createnode(int data){
	node* new=malloc(sizeof(node));
	new->data=data;
	new->color=RED;
	new->left=new->right=new->parent=NULL;
	return new;
}
node* insert(REDBLACKTREE* tree, int data)
{
	if(tree->root==NULL){	
		tree->root=createnode(data);
		tree->root->color=BLACK;
		status=SUCCESS;
		return tree->root;
	}
	else{
		node* new=createnode(data);
		node* temp=tree->root;
		node* prev=NULL;
		while(temp!=NULL){
			if(data > temp->data){
				prev=temp;
				temp=temp->right;
			}
			else if(data< temp->data){
				prev=temp;
				temp=temp->left;
			}
			else{
				status=DUPLICATE;
				free(new);
				return tree->root;
			}
		}
		new->parent=prev;
		if(prev->data > data){
			prev->left=new;
		}
		else{
			prev->right=new;
		}	
		insertfixup(tree,new);
		status=SUCCESS;
		printf("returning from inser\n");
		return tree->root;
	}
	printf("returning from inser\n");
}
