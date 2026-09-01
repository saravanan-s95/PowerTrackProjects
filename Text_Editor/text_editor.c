/*
Name : Manjusha Thiruveedhula
Registration Number : 25048_005
Description : This project is a menu-driven Text Editor developed in C using the Doubly Linked List data structure. 
              It allows users to perform basic text editing operations such as insert, delete, search, 
              replace, copy, paste, undo, and redo. The project also supports cursor navigation and file handling operations 
              like opening and saving text files. It is designed to demonstrate the practical implementation of linked lists, 
              stacks, and file handling concepts in Data Structures.

              1. Insert Line
              The Insert Line option is used to add a new line of text to the editor. 
              When the user enters a line, a new node is created using the doubly linked list, and the node is 
              inserted at the end of the list. The inserted operation is also stored in the Undo stack so that it 
              can be reversed later if required.
              
              2. Delete Line
              The Delete Line option removes a specified line from the editor. 
              The program searches for the required line, removes the corresponding node from the doubly linked 
              list, and updates the previous and next links of the neighboring nodes. 
              The deleted information is stored in the Undo stack so that the deleted line can be restored using the Undo operation.

              3. Display Text
              The Display Text option displays all the lines currently present in the text editor.
              It traverses the doubly linked list from the head node to the tail node and prints each line 
              in the order in which it is stored.

              4. Search Text
              The Search Text option allows the user to search for a particular word or line in the editor. 
              The linked list is traversed sequentially, and each line is compared with the user input. 
              If the text is found, the program displays a success message; otherwise, it informs the user that the text is not present.

              5. Replace Text
              The Replace Text option is used to replace an existing word with a new word. 
              The program searches each line for the specified word and, if found, replaces it with the new word entered by the user. 
              After replacement, the updated line is stored back into the linked list.

              6. Copy Line
              The Copy Line option copies a selected line into a temporary clipboard without removing it from the editor. 
              The copied text remains available until another copy or cut operation is performed. 
              This clipboard is later used by the Paste operation.

              7. Paste Line
              The Paste Line option inserts the contents of the clipboard into the editor. 
              A new node is created with the copied or cut text and inserted into the linked list. 
              If the clipboard is empty, the program displays an appropriate message.

              8. Undo
              The Undo option reverses the most recent editing operation performed by the user. 
              It retrieves the latest action from the Undo stack, performs the opposite operation, 
              and stores that action in the Redo stack. This allows the user to recover the undone action 
              later if required.
              
              9. Redo
              The Redo option restores the operation that was previously undone. 
              It retrieves the latest action from the Redo stack, performs that action again, and stores it back 
              in the Undo stack. This enables the user to reapply changes that were removed using Undo.
              
              10. Move Cursor Up
              The Move Cursor Up option moves the cursor to the previous line in the editor. 
              If the cursor is already at the first line, the program informs the user that no further upward movement is possible.
              
              11. Move Cursor Down
              The Move Cursor Down option moves the cursor to the next line in the editor. 
              If the cursor is already positioned at the last line, the program displays a message indicating 
              that it cannot move further down.

              12. Show Cursor
              The Show Cursor option displays the current position of the cursor along with the text present at that position. 
              This helps the user identify the line on which the cursor is currently placed.
              
              13. Open File
              The Open File option loads the contents of an existing text file into the editor. 
              The program reads the file line by line, creates a node for each line, and stores all the lines in 
              the doubly linked list so that they can be edited.

              14. Save File
              The Save File option stores the current contents of the editor into a text file. 
              The linked list is traversed from the beginning to the end, and every line is written to the 
              specified file. This ensures that all modifications made by the user are permanently saved.
              
              15. Close File
              The Close File option closes the currently opened file after all necessary operations are completed. 
              It indicates that file-related operations have been finished successfully.
              
              16. Exit
              The Exit option terminates the execution of the text editor. 
              Before exiting, the user is expected to save any important changes to avoid losing data. 
              The program then closes all active operations and ends normally.
*/


#include "text_editor.h"
#include "text_operations.h"
#include "display_text.h"
#include "stack.h"
#include "redo_undo_text.h"
#include "cursor_point.h"
#include "file.h"


int main()
{
    Node *head = NULL;
    Node *tail = NULL;

    Stack undoStack;
    Stack redoStack;

    Cursor cursor;

    Action action;

    char text[MAX_LINE_LENGTH];
    char filename[50];
    char old_word[50];
    char new_word[50];

    int choice;
    int line_no;

    init_stack(&undoStack);
    init_stack(&redoStack);
    init_cursor(&cursor, head);


    while(1)
    {
        printf("\n=====================================\n");
        printf("          TEXT EDITOR\n");
        printf("=====================================\n");

        printf("1. Insert Line\n");
        printf("2. Delete Line\n");
        printf("3. Display Text\n");
        printf("4. Search Text\n");
        printf("5. Replace Text\n");
        printf("6. Copy Line\n");
        printf("7. Paste Line\n");
        printf("8. Undo\n");
        printf("9. Redo\n");
        printf("10. Move Cursor Up\n");
        printf("11. Move Cursor Down\n");
        printf("12. Show Cursor\n");
        printf("13. Open File\n");
        printf("14. Save File\n");
        printf("15. Close File\n");
        printf("16. Exit\n");

        printf("\nEnter Choice : ");
        scanf("%d",&choice);
        getchar();

        switch(choice)
        {
            case 1:
                printf("Enter Line : ");
                fgets(text, MAX_LINE_LENGTH, stdin);

                text[strcspn(text,"\n")] = '\0';

                insert_line(&head,&tail,text);

                if(cursor.current == NULL)
                {
                    init_cursor(&cursor, head);
                }

                strcpy(action.operation,"insert");

                strcpy(action.text,text);

                action.line_no = get_line_count(head);

                push(&undoStack,action);

                printf("Line Inserted Successfully\n");

            break;

            case 2:
                printf("Enter Line Number : ");
                scanf("%d",&line_no);
                getchar();

                strcpy(action.operation,"delete");

                action.line_no=line_no;

                if(delete_line(&head,&tail,line_no,action.text))
                {
                    push(&undoStack,action);
                }

            break;

            case 3:
                display_text(head);
            break;

            case 4:

                printf("Enter Word : ");
                fgets(text,MAX_LINE_LENGTH,stdin);

                text[strcspn(text,"\n")] = '\0';

                search_text(head,text);

            break;


            case 5:

                printf("Enter Old Word : ");
                fgets(old_word,50,stdin);

                old_word[strcspn(old_word,"\n")] = '\0';

                printf("Enter New Word : ");
                fgets(new_word,50,stdin);

                new_word[strcspn(new_word,"\n")] = '\0';

                replace_text(head,old_word,new_word);

            break;


            case 6:

                printf("Enter Line Number : ");
                scanf("%d",&line_no);
                getchar();

                copy_text(head,line_no);

            break;

            case 7:

                paste_text(&head, &tail);

                strcpy(action.operation,"insert");
                strcpy(action.text, clipboard);

                action.line_no = get_line_count(head);

                push(&undoStack, action);

            break;


            case 8:

                undo(&head,&tail,&undoStack,&redoStack);;

            break;


            case 9:

                redo(&head,&tail,&undoStack,&redoStack);

            break;


            case 10:

                move_up(&cursor);

            break;


            case 11:

                move_down(&cursor);

            break;


            case 12:

                show_cursor(&cursor);

            break;


            case 13:

                printf("Enter File Name : ");

                scanf("%s", filename);

                open_file(&head, &tail, filename);

                init_cursor(&cursor, head);

            break;


            case 14:

                printf("Enter File Name : ");
                scanf("%s", filename);

                save_file(head, filename);

            break;


            case 15:

                close_file();

            break;


            case 16:

                printf("Thank You...\n");

                exit(0);

            default:

                printf("Invalid\n");
        }
    }
}