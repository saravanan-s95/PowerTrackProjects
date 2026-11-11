#include<stdio.h>
#include<stdlib.h>
#include "red_black_tree.h"

int main()
{
    tree_t *root;
    init_tree(&root);
    int choice;
    data_t element, min, max; 
    char ch;

    while(1)
    {
        printf("1.Create a tree\n");
        printf("2.Display\n");
        printf("3.Search a node\n");
        printf("4.Find Maximum node in Tree\n");
        printf("5.Find Minimum node in Tree\n");
        printf("6.Deletion\n");
        printf("7.Delete Minimum node\n");
        printf("8.Delete Maximum Node\n");
        printf("9.Exit\n");
        printf("Enter the choice : ");
        scanf("%d", &choice); 

        switch(choice)
        {
            case 1:
            

            do
            {
                printf("\nEnter the element :");
                scanf("%d", &element);

                insertion(&root, element);

                printf("\nDo u want to contiune(y/n) : ");
                scanf(" %c", &ch);
            
            }while(ch == 'y' || ch == 'Y');
            
            break;
        
        

            case 2:
                printf("Display :");
                inorder_display(root);
                printf("\n");
                break;

            case 3:
                printf("Enter the element which u want to search from the red black tree : ");
                scanf("%d", &element);
                
                
                {
                    tree_t *temp = root;
                    while(temp != NIL && temp->data != element)
                    {
                        if(element < temp->data)
                            temp = temp->left;
                        else    
                            temp = temp->right;    
                    }

                    if(temp != NIL && temp->colour == RED)
                    {
                        printf("Node is found (%d)--(RED->%d)\n", temp->data, RED);
                    }
                    else if(temp != NIL && temp->colour == BLACK)
                    {
                        printf("Node is found (%d)--(BLACK->%d)\n", temp->data, BLACK);
                    }
                    else 
                    {
                        printf("Node not found (%d)\n", element);

                    }
                    break;
                }

            case 4:
                data_t max; 
                find_maximum(&root,&max);
                tree_t *temp = root; 
                while(temp->right!=NIL) 
                {
                    temp = temp->right;
                }
                printf("Maximum node in the given Red Black Tree is : (%d)--(%s->%d)\n",max,temp->colour==RED?"RED":"BLACK",temp->colour);
                break;

            case 5:
                data_t min; 
                find_minimum(&root,&min);
                tree_t *t = root; 
                while(t->left!=NIL) 
                    t = t->left;
                printf("Minimum node in the given Red Black Tree is : (%d)--(%s->%d)\n",min,t->colour==RED?"RED":"BLACK",t->colour);
                break;

            case 6:
                printf("Enter the node which u want to delete: ");
                scanf("%d",&element);
                tree_t *tmp = root;
                while(tmp != NIL && tmp->data != element)
                { 
                    if(element < tmp->data) 
                        tmp = tmp->left; 
                    else 
                        tmp = tmp->right; 
                }
                if(tmp == NIL)
                    printf("Node not found\n");
                else
                {    
                    if(tmp->right != NIL)
                    {
                        tree_t *s=tmp->right; 
                        while(s->left!=NIL) 
                            s=s->left;
                        printf("Successor data : %d\n",s->data);
                    }
                    delete(&root,element);
        
                }
                
                break;
                    
            case 7:
                printf("Tree Before deleting the minimum node:\n");
                inorder_display(root); 
                printf("\n");
                delete_minimum(&root);
                printf("Tree After deleting the minimum node:\n");
                inorder_display(root); 
                printf("\n");
                break;    
              
            case 8:
                printf("Tree Before deleting the maximum node:\n");
                inorder_display(root); 
                printf("\n");
                delete_maximum(&root);
                printf("Tree After deleting the maximum node\n");
                inorder_display(root);
                printf("\n");
                break;

            case 9:
                exit(0);
                   

            case 10:
                printf("Invalid choice\n");
                break;
    
                
                




        }
    }
}
    
    

