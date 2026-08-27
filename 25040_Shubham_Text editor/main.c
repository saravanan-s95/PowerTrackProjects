#if 0
Project Name:Text editor
Name: Shubham Maroti Jagtap
Date: 03/08/2026
description: 
This project implements a console-based text editor written in C that provides essential text editing capabilities with 
advanced features like undo/redo functionality. The editor uses a doubly linked list data structure to manage lines of 
text and dynamic array stacks for implementing undo/redo operations. The project demonstrates key concepts in data structures 
including linked lists, stacks, and dynamic memory management, making it an excellent example of applying fundamental computer 
science concepts to build a practical application.
#endif
#include "file_ops.h"
#include "redo_undo.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "display_search.h"
DynamicArrayStack undoStack;
DynamicArrayStack redoStack;
void handleCommand(char *str,TextEditor *editor);
void delete_text_editor(TextEditor *editor);
void printMenu();
void init(TextEditor *editor,DynamicArrayStack *undo,DynamicArrayStack *redo)
{
    editor->head=NULL;
    editor->tail=NULL;
    editor->cursorLine=0;
    editor->cursorPos=0;
    editor->cursor=NULL;

    undo->capacity=20;
    undo->size=-1;
    //undo->actions->cursorLine=0;
    undo->actions = malloc(50*sizeof(Action));

    redo->actions=malloc(50*sizeof(Action));

    redo->capacity=50;
    redo->size=-1;
}
int main(int count,char *argv[])
{
    // Initialize the text editor and stacks
    TextEditor editor;
    init(&editor, &undoStack, &redoStack);
    if(count > 1)
    {
        load_from_file(argv,&editor);
    }
    

    // User interaction loop
    char command[256];

    while (1) 
    {
        printMenu();
        printf("Enter command: ");
        //fgets(command, sizeof(command), stdin);
        scanf(" %[^\n]",command);
        __fpurge(stdin);
         //TODO Remove newline character

        if(strcmp(command, "exit") == 0) 
        {
            char ch;
            printf("Do you want to save TextEditor file (y/n): ");
            scanf(" %c",&ch);
            if(ch == 'y')
            {
                if(count>1)
                {
                    save_into_file(argv[1],&editor);
                }
                else
                {
                    char str[20];
                    printf("Give the file Name: ");
                    scanf(" %s",str);
                    save_into_file(str,&editor);
                }
            }
            delete_text_editor(&editor);
            break;
        } 
        else 
        {
            handleCommand(command, &editor);
        }
    }

    return 0;
}
void printMenu() {
    printf("\n=================== Text Editor Commands ==================================\n\n");
    printf("insert <text>         - Insert text at the current cursor position\n");
    printf("delete <num_chars>    - Delete characters from the current cursor position\n");
    printf("undo                  - Undo the last operation\n");
    printf("redo                  - Redo the last undone operation\n");
    printf("print                 - Display the current text\n");
    printf("navigation            - Open cursor navigation menu\n");
    printf("exit                  - Exit the text editor\n");
    printf("==============================================================================\n");
}
