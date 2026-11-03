
#include <stdio.h>
#include <stdlib.h>

#include "text_editor.h"
#include "text_ops.h"
#include "undo_redo.h"
#include "file_ops.h"
#include "search_replace.h"

#define SUCCESS 0
#define FAILURE -1

int main(void)
{
    Node *head = NULL;
    Node *tail = NULL;
    Node *cursor = NULL;

    Stack undo_stack;
    Stack redo_stack;

    int choice;
    int line_number;
    int position;
    int result;
    int i;
    char text[1024];
    char filename[256];
    char old_text[256];
    char new_text[256];

    if (init_stack(&undo_stack) != SUCCESS)
    {
        printf("Undo stack initialization failed.\n");
        return FAILURE;
    }

    if (init_stack(&redo_stack) != SUCCESS)
    {
        printf("Redo stack initialization failed.\n");
        free(undo_stack.array);
        return FAILURE;
    }

    do
    {
        printf("\n========== TEXT EDITOR ==========\n");
        printf("1. Display document\n");
        printf("2. Insert line\n");
        printf("3. Delete line\n");
        printf("4. Insert character\n");
        printf("5. Delete character\n");
        printf("6. Copy line\n");
        printf("7. Cut line\n");
        printf("8. Paste line\n");
        printf("9. Undo\n");
        printf("10. Redo\n");
        printf("11. Search text\n");
        printf("12. Replace text\n");
        printf("13. Open file\n");
        printf("14. Save file\n");
        printf("15. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            break;
        }

        getchar();

        switch (choice)
        {
            case 1:
                if (head == NULL)
                    printf("Document is empty.\n");
                else
                    display_document(head);
                break;

            case 2:
                printf("Enter line number to insert: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();

                printf("Enter text: ");

                if (fgets(text, sizeof(text), stdin) == NULL)
                    break;

                i = 0;
                while (text[i] != '\n' && text[i] != '\0')
                    i++;
                text[i] = '\0';

                result = insert_line(&head, &tail, line_number, text);

                if (result == SUCCESS)
                {
                    undo_stack.top = -1;
                    redo_stack.top = -1;
                    printf("Line inserted successfully.\n");
                }
                else
                    printf("Line insertion failed.\n");
                break;

            case 3:
                printf("Enter line number to delete: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();

                result = delete_line(&head, &tail, line_number);

                if (result == SUCCESS)
                {
                    undo_stack.top = -1;
                    redo_stack.top = -1;
                    printf("Line deleted successfully.\n");
                }
                else
                    printf("Line deletion failed.\n");
                break;

            case 4:
                printf("Enter line number: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();
                cursor = get_node(head, line_number);

                if (cursor == NULL)
                {
                    printf("Line not found.\n");
                    break;
                }

                printf("Enter character position (0-based): ");

                if (scanf("%d", &position) != 1)
                {
                    printf("Invalid position.\n");
                    break;
                }

                getchar();

                printf("Enter one character: ");

                if (fgets(text, sizeof(text), stdin) == NULL)
                    break;

                if (text[0] == '\n' || text[0] == '\0' ||
                    (text[1] != '\n' && text[1] != '\0'))
                {
                    printf("Enter exactly one character.\n");
                    break;
                }

                result = insert_char_with_undo(
                    cursor, line_number, position, text[0],
                    &undo_stack, &redo_stack);

                if (result == SUCCESS)
                    printf("Character inserted successfully.\n");
                else
                    printf("Character insertion failed.\n");
                break;

            case 5:
                printf("Enter line number: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();
                cursor = get_node(head, line_number);

                if (cursor == NULL)
                {
                    printf("Line not found.\n");
                    break;
                }

                printf("Enter character position to delete (0-based): ");

                if (scanf("%d", &position) != 1)
                {
                    printf("Invalid position.\n");
                    break;
                }

                getchar();

                result = delete_char_with_undo(
                    cursor, line_number, position,
                    &undo_stack, &redo_stack);

                if (result == SUCCESS)
                    printf("Character deleted successfully.\n");
                else
                    printf("Character deletion failed.\n");
                break;

            case 6:
                printf("Enter line number to copy: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();
                cursor = get_node(head, line_number);

                if (cursor == NULL)
                    printf("Line not found.\n");
                else if (copy_line(cursor) == SUCCESS)
                    printf("Line copied successfully.\n");
                else
                    printf("Copy failed.\n");
                break;

            case 7:
                printf("Enter line number to cut: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();
                cursor = get_node(head, line_number);

                if (cursor == NULL)
                    printf("Line not found.\n");
                else if (cut_line(&head, &tail, cursor) == SUCCESS)
                {
                    undo_stack.top = -1;
                    redo_stack.top = -1;
                    printf("Line cut successfully.\n");
                }
                else
                    printf("Cut failed.\n");
                break;

            case 8:
                printf("Enter line number to paste at: ");

                if (scanf("%d", &line_number) != 1)
                {
                    printf("Invalid line number.\n");
                    break;
                }

                getchar();

                if (paste_line(&head, &tail, line_number) == SUCCESS)
                {
                    undo_stack.top = -1;
                    redo_stack.top = -1;
                    printf("Line pasted successfully.\n");
                }
                else
                    printf("Paste failed.\n");
                break;

            case 9:
                if (undo_operation(&undo_stack, &redo_stack, head) == SUCCESS)
                    printf("Undo successful.\n");
                else
                    printf("Nothing to undo.\n");
                break;

            case 10:
                if (redo_operation(&undo_stack, &redo_stack, head) == SUCCESS)
                    printf("Redo successful.\n");
                else
                    printf("Nothing to redo.\n");
                break;

            case 11:
                printf("Enter text to search: ");

                if (fgets(text, sizeof(text), stdin) == NULL)
                    break;

                i = 0;
                while (text[i] != '\n' && text[i] != '\0')
                    i++;
                text[i] = '\0';

                result = search_text(head, text);

                if (result == 0)
                    printf("Text not found.\n");
                else if (result == FAILURE)
                    printf("Invalid search text.\n");
                break;

            case 12:
                printf("Enter text to replace: ");

                if (fgets(old_text, sizeof(old_text), stdin) == NULL)
                    break;

                i = 0;
                while (old_text[i] != '\n' && old_text[i] != '\0')
                    i++;
                old_text[i] = '\0';

                printf("Enter replacement text: ");

                if (fgets(new_text, sizeof(new_text), stdin) == NULL)
                    break;

                i = 0;
                while (new_text[i] != '\n' && new_text[i] != '\0')
                    i++;
                new_text[i] = '\0';

                result = replace_text(head, old_text, new_text);

                if (result >= 0)
                {
                    printf("Replacements made: %d\n", result);

                    if (result > 0)
                    {
                        undo_stack.top = -1;
                        redo_stack.top = -1;
                    }
                }
                else
                    printf("Replace failed.\n");
                break;

            case 13:
                printf("Enter filename to open: ");

                if (fgets(filename, sizeof(filename), stdin) == NULL)
                    break;

                i = 0;
                while (filename[i] != '\n' && filename[i] != '\0')
                    i++;
                filename[i] = '\0';

                if (open_file(&head, &tail, filename) == SUCCESS)
                {
                    undo_stack.top = -1;
                    redo_stack.top = -1;
                    printf("File opened successfully.\n");
                }
                else
                    printf("Unable to open file.\n");
                break;

            case 14:
                printf("Enter filename to save: ");

                if (fgets(filename, sizeof(filename), stdin) == NULL)
                    break;

                i = 0;
                while (filename[i] != '\n' && filename[i] != '\0')
                    i++;
                filename[i] = '\0';

                if (save_file(head, filename) == SUCCESS)
                    printf("File saved successfully.\n");
                else
                    printf("Unable to save file.\n");
                break;

            case 15:
                printf("Exiting text editor...\n");
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }

    } while (choice != 15);

    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }

    free(clipboard);
    clipboard = NULL;

    free(undo_stack.array);
    free(redo_stack.array);

    return SUCCESS;
}
