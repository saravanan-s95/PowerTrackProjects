
#include <stdio.h>
#include "rb_tree.h"

int main(void)
{
    tree_t *root = NULL;
    int choice, item, result;
    data_t value;
    char answer;

    while (1)
    {
        printf("\n1. Create a tree\n");
        printf("2. Display\n");
        printf("3. Search a node\n");
        printf("4. Find Maximum node in Tree\n");
        printf("5. Find Minimum node in Tree\n");
        printf("6. Deletion\n");
        printf("7. Delete Minimum node\n");
        printf("8. Delete Maximum node\n");
        printf("9. Exit\n");

        printf("Enter the choice: ");

        if (scanf("%d", &choice) != 1)
        {
            free_tree(root);
            return 1;
        }

        switch (choice)
        {
            case 1:
                do
                {
                    printf("Enter the element: ");

                    if (scanf("%d", &item) != 1)
                    {
                        free_tree(root);
                        return 1;
                    }

                    result = insert(&root, item);

                    if (result == SUCCESS)
                    {
                        printf("Element inserted successfully\n");
                        printf("Tree after balancing is:\n");
                        display(root);
                        printf("\n");
                    }
                    else
                    {
                        printf("Insertion failed: duplicate or memory error\n");
                    }

                    printf("Do you want to continue (y/n): ");

                    if (scanf(" %c", &answer) != 1)
                    {
                        free_tree(root);
                        return 1;
                    }

                } while (answer == 'y' || answer == 'Y');

                break;

            case 2:
                if (root == NULL)
                    printf("Tree is empty\n");
                else
                {
                    printf("Display: ");
                    display(root);
                    printf("\n");
                }
                break;

            case 3:
                printf("Enter the element which you want to search: ");

                if (scanf("%d", &item) != 1)
                {
                    free_tree(root);
                    return 1;
                }

                if (search(root, item) == SUCCESS)
                    printf("%d is found in the tree\n", item);
                else
                    printf("%d is not found in the tree\n", item);

                break;

            case 4:
                if (find_maximum(&root, &value) == SUCCESS)
                    printf("Maximum node in the given Red Black Tree is: %d\n",
                           value);
                else
                    printf("Tree is empty\n");

                break;

            case 5:
                if (find_minimum(&root, &value) == SUCCESS)
                    printf("Minimum node in the given Red Black Tree is: %d\n",
                           value);
                else
                    printf("Tree is empty\n");

                break;

            case 6:
                printf("Enter the node which you want to delete: ");

                if (scanf("%d", &item) != 1)
                {
                    free_tree(root);
                    return 1;
                }

                if (delete(&root, item) == SUCCESS)
                {
                    printf("Deletion successful\n");
                    printf("Tree after deletion: ");
                    display(root);
                    printf("\n");
                }
                else
                {
                    printf("Element not found in the tree\n");
                }

                break;

            case 7:
                if (delete_minimum(&root) == SUCCESS)
                {
                    printf("Tree after deleting the minimum node: ");
                    display(root);
                    printf("\n");
                }
                else
                {
                    printf("Tree is empty\n");
                }

                break;

            case 8:
                if (delete_maximum(&root) == SUCCESS)
                {
                    printf("Tree after deleting the maximum node: ");
                    display(root);
                    printf("\n");
                }
                else
                {
                    printf("Tree is empty\n");
                }

                break;

            case 9:
                free_tree(root);
                printf("Exiting program\n");
                return 0;

            default:
                printf("Invalid choice\n");
                break;
        }
    }

    free_tree(root);
    return 0;
}
