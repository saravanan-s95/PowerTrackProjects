/* Name : Riddhi Balaji Shitole 
   ID : 25034f-036
   PROJECT: Red-Black-Tree
*/
/**************************************************************************************************************************************************************
*Title          : main function(Driver function)
*Description    : This function is used as the driver function for all the functions
***************************************************************************************************************************************************************/
#include "rbt.h"

int main()
{
    tree_t *root = NULL;
    data_t data;
    data_t minimum;
    data_t maximum;

    int operation;
    char option;

    do
    {
       printf("\n1.Create a tree\n");
       printf("2.Display\n");
       printf("3.Search a node\n");
       printf("4.Find Maximum node in Tree\n");
       printf("5.Find Minimum node in Tree\n");
       printf("6.Deletion\n");
       printf("7.Delete Minimum node\n");
       printf("8.Delete Maximum Node\n");
       printf("9.Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &operation);

        switch (operation)
        {
            case 1:
                printf("Enter the data to be inserted into the RB Tree: ");
                scanf("%d", &data);

                int status = insert(&root, data);

                if (status == DUPLICATE)
                {
                     printf("Duplicate data not allowed\n");
                }
                else if (status == FAILURE)
                {
                     printf("Insertion failed\n");
                }
                break;

            case 2: // Display
                     print_tree(root);
                break;
            
            case 3: // Search
                    printf("Enter node to search: ");
                    scanf("%d",&data);

                    if(search(root,data))
                        printf("Node found\n");
                    else
                        printf("Node not found\n");
                break;
            
            case 4: // Find Maximum
                if (find_maximum(&root, &maximum) == SUCCESS)
                    printf("Maximum data: %d\n", maximum);
                else
                    printf("Tree is empty\n");

                    print_tree(root);
                break;
            
            case 5: // Find Minimum
                if (find_minimum(&root, &minimum) == SUCCESS)
                    printf("Minimum data: %d\n", minimum);
                else
                    printf("Tree is empty\n");

                print_tree(root);
    
                break;
            
            case 6: // Delete
                printf("Enter the data to be deleted from the RB Tree: ");
                scanf("%d", &data);

                if (delete(&root, data) == SUCCESS)
                    print_tree(root);
                else
                    printf("Data not found\n");
                break;
            
            case 7: // Delete Minimum
    
                if (delete_minimum(&root) == SUCCESS)
                    print_tree(root);
                else
                    printf("Tree is empty\n");
                break;

            case 8: // Delete Maximum
    
                if (delete_maximum(&root) == SUCCESS)
                    print_tree(root);
                else
                    printf("Tree is empty\n");
                break;
            
            case 9:
                   return 0;

            default:
                printf("Invalid Choice\n");
        }

        printf("\nDo u want to continue (y/n): ");
        scanf(" %c", &option);

    } while (option == 'y' || option == 'Y');

    return 0;
}