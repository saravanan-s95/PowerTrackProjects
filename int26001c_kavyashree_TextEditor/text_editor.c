/*
Name: Kavyashree.K
Date: 16/08/2026
Description: A console-based text editor in C implementing text manipulation,cursor navigation, 
            file operations, search/replace, clipboard management, and stack-based undo/redo using data structures.*/


#include "text_editor.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "file_ops.h"
#include "display_search.h"
#include "redo_undo.h"

/* Create and initialize a new node containing the given line of text. */
Node *createNode(const char *text)
{
    Node *newNode = malloc(sizeof(Node));    

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->text = malloc(strlen(text) + 1);     

    if (newNode->text == NULL)        
    {
        free(newNode);                
        return NULL;
    }

    strcpy(newNode->text, text);        

    newNode->prev = NULL;             
    newNode->next = NULL;

    return newNode;        
}

/* Free all text nodes and reset the editor's linked-list pointers. */
void freeAllNodes(TextEditor *editor)
{
    if (editor == NULL)       
        return;

    Node *temp = editor->head;      

    while (temp != NULL)            
    {
        Node *next = temp->next;

        free(temp->text);
        free(temp);

        temp = next;
    }

    editor->head = NULL;            
    editor->tail = NULL;
    editor->cursor = NULL;
}

/* Initialize the editor, process user commands, and release resources on exit. */
void initializeEditor(TextEditor *editor)
{
    editor->head = NULL;         
    editor->tail = NULL;
    editor->cursor = NULL;

    editor->cursorLine = 1;       
    editor->cursorPos = 0;

    editor->filename = NULL;         
    editor->clipboard = NULL;

    editor->modified = 0;           

    initializeStack(&editor->undoStack);         
    initializeStack(&editor->redoStack);

}

void printMenu(void)          
{
    printf("insert <text>       - Insert text\n");
    printf("delete <number>     - Delete characters\n");

    printf("up                  - Move cursor up\n");
    printf("down                - Move cursor down\n");
    printf("left                - Move cursor left\n");
    printf("right               - Move cursor right\n");

    printf("home                - Start of line\n");
    printf("end                 - End of line\n");
    printf("start               - Start of file\n");
    printf("finish              - End of file\n");

    printf("copy <number>       - Copy characters\n");
    printf("cut <number>        - Cut characters\n");
    printf("paste               - Paste clipboard\n");

    printf("delete_line         - Delete current line\n");

    printf("open <filename>     - Open file\n");
    printf("save <filename>     - Save file\n");
    printf("close               - Close file\n");

    printf("find <text>         - Search text\n");
    printf("replace <old> <new> - Replace text\n");

    printf("undo                - Undo\n");
    printf("redo                - Redo\n");

    printf("print               - Display text\n");
    printf("help                - Show menu\n");
    printf("exit                - Exit\n");

    printf("============================================\n");
}

int main()
{
    TextEditor editor;      

    char command[1024];      
    char *argument;                             

    initializeEditor(&editor);       

    printf("\n");
    printf("============================================\n");
    printf("             TEXT EDITOR \n");
    printf("============================================\n");

    printMenu();             
 
    while (1)
    {
        printf("\nEnter command: ");

        if (fgets(command, sizeof(command), stdin) == NULL)  
        {
            break;                      
        }

        command[strcspn(command, "\n")] = '\0';       

        if (strlen(command) == 0)       
        {     
            continue;
        }

        if (strncmp(command, "insert ", 7) == 0)       
        {
            argument = command + 7;               

            insertText(&editor, argument);     
        }

        else if (strncmp(command, "delete ", 7) == 0)     
        {
            int count = atoi(command + 7);        

            deleteCharacters(&editor, count);     
        }

        else if (strncmp(command, "copy ", 5) == 0)      
        {
            int count = atoi(command + 5);        

            copyText(&editor, count);
        }

        else if (strncmp(command, "cut ", 4) == 0)
        {
            int count = atoi(command + 4);

            cutText(&editor, count);
        }

        else if (strcmp(command, "paste") == 0)      
        {
            pasteText(&editor);
        }

        else if (strcmp(command, "delete_line") == 0)     
        {
            deleteLine(&editor);
        }

        else if (strcmp(command, "up") == 0)          
        {
            moveCursorUp(&editor);
        }

        else if (strcmp(command, "down") == 0)
        {
            moveCursorDown(&editor);
        }

        else if (strcmp(command, "left") == 0)
        {
            moveCursorLeft(&editor);
        }

        else if (strcmp(command, "right") == 0)
        {
            moveCursorRight(&editor);
        }

        else if (strcmp(command, "home") == 0)      
        {
            jumpToStartOfLine(&editor);
        }

        else if (strcmp(command, "end") == 0)          
        {
            jumpToEndOfLine(&editor);
        }

        else if (strcmp(command, "start") == 0)        
        {
            jumpToStartOfFile(&editor);
        }

        else if (strcmp(command, "finish") == 0)
        {
            jumpToEndOfFile(&editor);
        }

        else if (strcmp(command, "print") == 0)       
        {
            displayText(&editor);
        }

        else if (strcmp(command, "position") == 0)        
        {
            showCursorPosition(&editor);
        }

        else if (strncmp(command, "open ", 5) == 0)     
        {
            argument = command + 5;

            openFile(&editor, argument);
        }

        else if (strncmp(command, "save ", 5) == 0)     
        {
            argument = command + 5;

            saveFile(&editor, argument);
        }

        else if (strcmp(command, "close") == 0)         
        {
            closeFile(&editor);
        }

        else if (strncmp(command, "find ", 5) == 0)    
        {
            argument = command + 5;

            findText(&editor, argument);
        }

        else if (strncmp(command, "replace ", 8) == 0)      
        {
            char *oldText = command + 8;               

            char *newText = strchr(oldText, ' ');         

            if (newText == NULL)
            {
                printf("Usage: replace <old> <new>\n");
            }
            else
            {
                *newText = '\0';         
                newText++;

                replaceText(&editor,oldText,newText);
            }
        }

        else if (strcmp(command, "undo") == 0)
        {
            undo(&editor);
        }

        else if (strcmp(command, "redo") == 0)
        {
            redo(&editor);
        }

        else if (strcmp(command, "help") == 0)
        {
            printMenu();
        }

        else if (strcmp(command, "exit") == 0)
        {
            break;
        }

        else
        {
            printf("Error: Unknown command '%s'.\n", command);

            printf("Type 'help' for available commands.\n");
        }
    }
    
     
    freeAllNodes(&editor);

    free(editor.filename);
    free(editor.clipboard);

    clearStack(&editor.undoStack);
    clearStack(&editor.redoStack);

    printf("\nGoodbye! Thank you for using Text Editor.\n");

    return 0;
}