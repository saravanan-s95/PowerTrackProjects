/*******************************************************************************************************************************************************************
*Title			: Deletion
*Description		: This function performs deleting of the given data from the given Red Black tree.
*Prototype		: int delete(tree_t **root, data_t item); 
*Input Parameters	: root – Pointer to the root node of the Red Black tree.
			: item – Data to be deleted from the Red Black tree.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "rbt.h"

int delete(tree_t **root, data_t item) {
// Step 1:- Perform BST deletion
// Step 2:-
// Case 1:- if node to be deleted is red, just delete it.
// Case 2:- If root is DB, just remove DB
// Case 3:- if DB's sibling is black, & both its children are black
//    remove DB
//    Add black to its Parent (P)
//   (3.1) -> if P is Red it becomes black
//   (3.2) -> if P is black it becomes double black
//    Make sibling red.
//    if still DB exits, apply other cases.
// Case 4:- if DB's sibling is red
//    Swap colors of Parent & its Sibling
//   rotate Parent in DB direction
//    reapply cases
// Case 5:- DB's sibling is black, sibling's child who is far from DB is black, but near child to DB is red.
//    Swap color of DB's sibling & sibling's child who is near to DB
//    rotate sibling in opposite direction to DB
//    apply case 6.
// Case 6:- DB's sibling is black, far child is red
//    Swap color of Parent & sibling
//   rotate Parent in DB's direction
//    remove DB
//   change color of red child to black

			if(*root==NULL) return -1; 
						
			tree_t *target_node = *root;
			tree_t *parent_node = NULL;
			tree_t *db_node = NULL;

			while(target_node!=NULL && target_node->data!=item){
				parent_node = target_node;
				if(item < target_node->data) target_node = target_node->left;
				else target_node = target_node->right;
			}

			if(target_node == NULL)
				return -1;

			if(target_node->left != NULL && target_node->right != NULL) {
				tree_t *successor = target_node->right;
			tree_t *successor_parent = target_node;
					
				while(successor->left != NULL){
				successor_parent = successor;
				successor = successor->left;
				}
					
				target_node->data = successor->data;
			    target_node = successor;
				parent_node = successor_parent;
			}

			tree_t *replacement_node = (target_node->left != NULL) ? target_node->left : target_node->right;

			if(parent_node == NULL) *root = replacement_node; 
			else if(parent_node->left == target_node) parent_node->left = replacement_node;
			else parent_node->right = replacement_node;

			int deleted_color = target_node->color;
			free(target_node);

			 if(deleted_color == 0) { 
				tree_t *db_node = replacement_node; 
						
				while(db_node != *root && (db_node == NULL || db_node->color == 0))
				{
				   if(db_node == parent_node->left) {
						tree_t *sibling = parent_node->right;

						if(sibling != NULL && sibling->color == 1) {
						    sibling->color = 0; 
						    parent_node->color = 1;
						    left_rotate(root, find_parent(*root, parent_node), parent_node); 
						    sibling = parent_node->right; 
						}

						int is_near_child_black = (sibling == NULL || sibling->left == NULL || sibling->left->color == 0);
						int is_far_child_black  = (sibling == NULL || sibling->right == NULL || sibling->right->color == 0);

					  if(is_near_child_black && is_far_child_black) {
						if(sibling != NULL) sibling->color = 1; 
						db_node = parent_node; 
						parent_node = find_parent(*root, db_node); 
					   }
 						else 
				     	{
							if(is_far_child_black) {
							if(sibling->left != NULL) sibling->left->color = 0; 
							sibling->color = 1;
							right_rotate(root, parent_node, sibling); 
							sibling = parent_node->right; 
							}

							sibling->color = parent_node->color; 
								parent_node->color = 0;
							if(sibling->right != NULL) sibling->right->color = 0; 
								left_rotate(root, find_parent(*root, parent_node), parent_node); 			
								db_node = *root; 
						}
				   }
				 else 
				 {
					tree_t *sibling = parent_node->left;

					if(sibling != NULL && sibling->color == 1) {
						sibling->color = 0;
						parent_node->color = 1;
					right_rotate(root, find_parent(*root, parent_node), parent_node); 
						sibling = parent_node->left;
					}

				   int is_near_child_black = (sibling == NULL || sibling->right == NULL || sibling->right->color == 0);
				   int is_far_child_black  = (sibling == NULL || sibling->left == NULL || sibling->left->color == 0);

						if(is_near_child_black && is_far_child_black) {
							if(sibling != NULL) sibling->color = 1;
							db_node = parent_node;
							parent_node = find_parent(*root, db_node);
						}
						else {
						  if(is_far_child_black) {
							if(sibling->right != NULL) sibling->right->color = 0;
								sibling->color = 1;
							 left_rotate(root, parent_node, sibling);
							 sibling = parent_node->left;
							}

							sibling->color = parent_node->color;
							parent_node->color = 0;
							if(sibling->left != NULL) sibling->left->color = 0;
							 right_rotate(root, find_parent(*root, parent_node), parent_node);
											
							db_node = *root; 
						}
				   }
				}
			}
						
			if(db_node != NULL) {
				db_node->color = 0; 
				}
		return 0; 
}