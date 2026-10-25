#include "text_editor.h"
#include "cursor_navigations.h"
#include "undo_redo.h"
#include "file_ops.h"

int main()
{
    printf("\n=====================================\n");
    printf("      Welcome to Text Editor v1.0\n");
    printf("=====================================\n\n");

    /* Initialize the text editor */
    TextEditor editor;
    editor.head = NULL;
    editor.tail = NULL;
    editor.current = NULL;
    editor.cursorLine = 0;
    editor.cursorPos = 0;

    /* Initialize undo and redo stacks */
    DynamicArrayStack undoStack; 
    DynamicArrayStack redoStack;

    if(initialize_Undo_Redo_Stack(&undoStack,&redoStack) == FAILURE)
    {
        return FAILURE;
    }

    /* Declare file structure */
    File file;

    /* Print text editor menu */
    printMenu();
    int open_file_count = 0;
    int save_count = 0;

    while(1)
    {
        /* Read commands from stdin */
        char command[MAX_CHARS];
        printf("Enter the command : ");
        fgets(command,sizeof(command),stdin);
        command[strcspn(command, "\n")] = '\0';

        /* Handle commands */
        CommandType c = handleCommands(command);

        switch(c)
        {
            case INSERT_CURRENT_LINE:
            {
                /* Insert text at current cursor position */
                if(insertText(&editor,&undoStack,command+6) == SUCCESS)
                {
                    printf("Text inserted successfully in current line\n");
                    /* Reset redo stack */
                    reset_redo(&redoStack);
                }
            }
            break;

            case INSERT_NEWLINE:
            {
                /* Insert text at new line */
                if(insertTextnewline(&editor,&undoStack,command+7) == SUCCESS)
                {
                    printf("Text inserted successfully in newline\n");
                    /* Reset redo stack */
                    reset_redo(&redoStack);
                }
            }
            break;

            case DELETE_CHARACTERS_CURRENT_POS:
            {
                /* Delete chars from current cursor position */
                int del_char_count;
                printf("Enter the no of characters to be deleted : ");
                scanf("%d",&del_char_count);
                getchar();

                if(deleteCharacters(&editor,&undoStack,del_char_count) == SUCCESS)
                {
                    printf("%d characters successfully deleted\n",del_char_count);
                    /* Reset redo stack */
                    reset_redo(&redoStack);
                }
            }
            break;

            case DELETE_LINE:
            {
                /* Delete current line */
                if(deleteLine(&editor,&undoStack) == SUCCESS)
                {
                    printf("Line %d successfully deleted\n",editor.cursorLine);
                    /* Reset redo stack */
                    reset_redo(&redoStack);
                }
            }
            break;

            case CURSOR_NAVIGATION:
            {
                /* Display cursor navigation menu */
                printf("\n\nCursor Navigation Menu\n");
                printCursorMenu();

                while(1)
                {
                    /* Move cursor based on choice */
                    int cursor_choice;
                    printf("Enter the choice : ");
                    scanf("%d",&cursor_choice);
                    getchar();

                    switch(cursor_choice)
                    {
                        case 1:
                        /* Moves cursor one line above */
                        move_cursor_up(&editor);
                        break;

                        case 2:
                        /* Moves cursor one line below */
                        move_cursor_down(&editor);
                        break;

                        case 3:
                        /* Moves cursor one position left */
                        move_cursor_left(&editor);
                        break;

                        case 4:
                        /* Moves cursor one position right */
                        move_cursor_right(&editor);
                        break;

                        case 5:
                        /* Moves cursor to start of the current line */
                        start_of_line(&editor);
                        break;

                        case 6:
                        /* Moves cursor to end of the current line */
                        end_of_line(&editor);
                        break;

                        case 7:
                        /* Moves cursor to the start of the text editor */
                        start_of_text_editor(&editor);
                        break;

                        case 8:
                        /* Moves cursor to the end of the text editor */
                        end_of_text_editor(&editor);
                        break;

                        case 9:
                        /* Exits out of cursor navigaton menu and returns to text editor menu */
                        break;

                        default:
                        /* Invalid */
                        printf("Error : Invalid option.Please choose the options present only in Cursor navigation menu\n");
                    }
                    if(cursor_choice == 9)
                    {
                        break;
                    }
                }
            }
            break;

            case COPY:
            {
                /* Copy text */
                if(copyText(editor) == SUCCESS)
                {
                    printf("Text copied successfully\n");
                }
            }
            break;

            case CUT:
            {
                /* Cut text */
                if(cutText(&editor,&undoStack) == SUCCESS)
                {
                    printf("Text cut successfully\n");
                    reset_redo(&redoStack);
                }
            }
            break;

            case PASTE:
            {
                /* Paste text */
                if(pasteText(&editor,&undoStack) == SUCCESS)
                {   
                    printf("Text pasted successfully\n");
                    reset_redo(&redoStack);
                }
            }   
            break;

            case PRINT:
            {
                /* Print text editor */
                printTextEditor(editor);
            }
            break;

            case UNDO:
            {
                /* Undo */
                if(undo_operation(&editor,&undoStack,&redoStack) == SUCCESS)
                {
                    printf("Undo successful\n");
                }
            }
            break;

            case REDO:
            {
                /* Redo */
                if(redo_operation(&editor,&undoStack,&redoStack) == SUCCESS)
                {
                    printf("Redo successful\n");
                }
            }
            break;

            case OPEN_FILE:
            {
                /* Check if text editor is empty and file loaded or not */
                if(editor.head != NULL || open_file_count >= 1)
                {
                    printf("Error : File can be loaded only once and text editor has to be empty\n");
                    break;
                }
                /* Open file */
                if(validate_file_name(&file) == SUCCESS)
                {
                    if(open_and_load_file(&editor,&undoStack,&file) == SUCCESS)
                    {
                        printf("File opened successfully\n");
                        open_file_count++;
                    }
                }
            }
            break;

            case SAVE_FILE:
            {
                /* Save file */
                if(validate_file_name(&file) == SUCCESS)
                {
                    if(save_file(editor,&file) == SUCCESS)
                    {
                        printf("File saved successfully\n");
                        save_count++;
                    }
                }
            }
            break;

            case EXIT:
            {
                if(save_count < 1)
                {
                    int s;
                    printf("Do you want to save the contents to file?(1-Yes/0-No) : ");
                    scanf("%d",&s);

                    if(s)
                    {
                        /* Save file */
                        if(validate_file_name(&file) == SUCCESS)
                        {
                            if(save_file(editor,&file) == SUCCESS)
                            {
                                printf("File saved successfully\n");
                                save_count++;
                            }
                        }
                    }
                }

                /* Delete text editor and undo and redo stacks and exit the program */
                delete_text_editor(&editor,&undoStack,&redoStack);
                printf("Goodbye!Thank you for using Text Editor v1.0\n\n");
            }
            return 0;

            default:
            /* Invalid */
            printf("Error : Invalid option.Please enter a valid option\n");
        }
    }
}