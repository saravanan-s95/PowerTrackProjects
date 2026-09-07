#include "rbt.h"

int main(void)
{
    tree_t *root = NULL;
    int choice;
    data_t item;
    data_t value;

    while (1)
    {
        printf("\n");
        printf("====================================\n");
        printf("       RED BLACK TREE MENU\n");
        printf("====================================\n");
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Search\n");
        printf("4. Find Minimum\n");
        printf("5. Find Maximum\n");
        printf("6. Delete Minimum\n");
        printf("7. Delete Maximum\n");
        printf("8. Print Tree\n");
        printf("9. Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("Enter the data to insert: ");
                scanf("%d", &item);

                if (insert(&root, item) == SUCCESS)
                {
                    printf("Data inserted successfully.\n");
                }
                else
                {
                    printf("Insertion failed. Duplicate data or memory allocation failure.\n");
                }
                break;

            case 2:
                printf("Enter the data to delete: ");
                scanf("%d", &item);

                if (delete(&root, item) == SUCCESS)
                {
                    printf("Data deleted successfully.\n");
                }
                else
                {
                    printf("Data not found.\n");
                }
                break;

            case 3:
                printf("Enter the data to search: ");
                scanf("%d", &item);

                if (search(root, item) == SUCCESS)
                {
                    printf("Data found in the tree.\n");
                }
                else
                {
                    printf("Data not found in the tree.\n");
                }
                break;

            case 4:
                if (find_minimum(&root, &value) == SUCCESS)
                {
                    printf("Minimum value: %d\n", value);
                }
                else
                {
                    printf("Tree is empty.\n");
                }
                break;

            case 5:
                if (find_maximum(&root, &value) == SUCCESS)
                {
                    printf("Maximum value: %d\n", value);
                }
                else
                {
                    printf("Tree is empty.\n");
                }
                break;

            case 6:
                if (delete_minimum(&root) == SUCCESS)
                {
                    printf("Minimum value deleted successfully.\n");
                }
                else
                {
                    printf("Tree is empty.\n");
                }
                break;

            case 7:
                if (delete_maximum(&root) == SUCCESS)
                {
                    printf("Maximum value deleted successfully.\n");
                }
                else
                {
                    printf("Tree is empty.\n");
                }
                break;

            case 8:
                if (root == NULL)
                {
                    printf("Tree is empty.\n");
                }
                else
                {
                    printf("Tree contents (Inorder):\n");
                    print_tree(root);
                }
                break;

            case 9:
                printf("Exiting...\n");
                return SUCCESS;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}