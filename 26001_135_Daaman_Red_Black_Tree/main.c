#include "rbt.h"

int main()
{
    // Initialising a tree
    Tree *root = NULL;

    // Printing menu
    while (1)
    {
        printf("1. Insert an element into Tree\n");
        printf("2. Display Tree\n");
        printf("3. Search a Node\n");
        printf("4. Find Maximum Node in Tree\n");
        printf("5. Find Minumum Node in Tree\n");
        printf("6. Delete an element from Tree\n");
        printf("7. Delete Minimum Node\n");
        printf("8. Delete Maximum Node\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            printf("Enter the element: ");
            int data;
            scanf("%d", &data);
            if (insert(&root, data) == DUPLICATE)
            {
                printf("ERROR: Duplicates not allowed\n");
            }
            break;
        }

        case 2:
        {
            display(root);
            break;
        }

        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            break;
        }
        case 6:
        {
            break;
        }
        case 7:
        {
            break;
        }
        case 8:
        {
            break;
        }
        case 9:
            exit(0);
            break;

        default:
            printf("Invalid choice\n");
        }
    }

    return 0;
}