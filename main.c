#include "text_editor.h"

int main()
{
    struct Node *head = NULL;
    int choice;

    while (1)
    {
        printf("\n========== TEXT EDITOR ==========\n");
        printf("1. Insert Text\n");
        printf("2. Search Text\n");
        printf("3. Display Text\n");
        printf("4. Delete Text\n");
        printf("5. Update Text\n");
        printf("6. Save Text\n");
        printf("7. Load Text\n");
        printf("8. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                insert_text(&head);
                break;

            case 2:
            {
                char str[SIZE];

                printf("Enter text to search: ");
                scanf(" %[^\n]", str);

                if (search_text(head, str))
                    printf("Text found\n");
                else
                    printf("Text not found\n");

                break;
            }

            case 3:
                display_text(head);
                break;

            case 4:
                delete_text(&head);
                break;

            case 5:
                update_text(head);
                break;

            case 6:
                save_text(head);
                break;

            case 7:
                load_text(&head);
                break;

            case 8:
                printf("Exiting Text Editor...\n");
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}