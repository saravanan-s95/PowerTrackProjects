/*
NAME: M CHANDANA
DATE: 03-08-2026
BATCH: 26001B
DESCRIPTION: Developed a Red-Black Tree implementation in C, incorporating self-balancing insertion and deletion opeartions using
rotations and recoloring techniques. Implemented core tree operations such as insertion, deletion, finding minimum and maximum
elements, deleting minimum and maximum nodes, and inorder traversal. Designed a menu-driven application with input validation and 
Makefile-based build system to efficiently test and manage Red-Black Tree operations.
*/

#include "main.h"

/*color coding macro*/
#define RED_ "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

int main()
{
    tree_t *root = NULL;
    int choice;
    data_t item;
    data_t min, max;

    while (1)
    {
        printf("\n");
        printf(BLUE "==========================================\n"RESET);
        printf("          RED BLACK TREE MENU\n");
        printf(BLUE"==========================================\n" RESET);
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Find Minimum\n");
        printf("4. Find Maximum\n");
        printf("5. Delete Minimum\n");
        printf("6. Delete Maximum\n");
        printf("7. Display\n");
        printf("8. Exit\n");
        printf(BLUE "==========================================\n" RESET);

        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice)
        {
        case 1:
            printf("Enter the element to insert: \n");

            //validate and read the input
            if (read_integer(&item) == FAILURE)
            {
                printf(RED_"Invalid input! Please enter digits only\n"RESET);
                break;
            }
            //insert the node
            if (insert(&root, item) == SUCCESS)
            {
                printf(GREEN "Insertion Successful\n" RESET);
                inorder(root);
                printf("\n");
            }
            else
                printf(RED_ "Duplicate element not allowed\n" RESET);
            break;

        case 2:
            //empty tree
            if (root == NULL)
            {
                printf(RED_ "Tree is Empty\n" RESET);
                break;
            }

            printf("Enter the element to delete: ");
            //validate and read the input
            if (read_integer(&item) == FAILURE)
            {
                printf(RED_"Invalid input! Please enter digits only\n"RESET);
                break;
            }

            //delete the node
            if (delete(&root, item) == SUCCESS)
            {
                printf(GREEN "Deletion Successful\n" RESET);
                inorder(root);
                printf("\n");
            }
            else
                printf(RED_ "Element not found\n" RESET);
            break;

        case 3:
            //find minimum node
            if (find_minimum(&root, &min) == SUCCESS)
            {
                printf("Minimum element = %d\n", min);
            }
            else
                printf(RED_ "Tree is Empty\n" RESET);

            break;

        case 4:
            //find maximum node
            if (find_maximum(&root, &max) == SUCCESS)
            {
                printf("Maximum element = %d\n", max);
            }
            else
                printf(RED_ "Tree is Empty\n" RESET);

            break;

        case 5:
            //delete minimum node
            if (delete_minimum(&root) == SUCCESS)
            {
                printf(GREEN "Minimum node deleted successfully\n" RESET);
            }
            else
            {
                printf(RED_ "Tree is Empty\n" RESET);
            }
            break;

        case 6:
            //delete maximum node
            if (delete_maximum(&root) == SUCCESS)
            {
                printf(GREEN "Maximum node deleted successfully\n" RESET);
            }
            else
            {
                printf(RED_ "Tree is Empty\n" RESET);
            }
            break;

        case 7:
            if (root == NULL)
            {
                printf(RED_ "Tree is Empty\n" RESET);
            }
            
            //display the node
            else
            {
                printf(GREEN "Inorder Traversal: \n" RESET);
                inorder(root);
                printf("\n");
            }
            break;

        case 8:
            printf(CYAN "Exiting...\n" RESET);
            return SUCCESS;

        default:
            printf(RED_ "Invalid Choice! Please try again.\n" RESET);
        }
    }

    return SUCCESS;
}