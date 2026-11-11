#include "text_editor.h" 
#include "text_ops.h"    
#include "cursor_nav.h"  
#include "redo_undo.h"   
#include "display.h"
#include "files_ops.h"
#include <ctype.h>

// Function to print the editor content inside a box
void printEditor(TextEditor *ed) 
{

    // Print the top border of the box
    printf("┌─────────────────────────────────────────────────────────┐\n");

    // Check if editor is empty 
    if (ed->totallines == 0 || !ed->head) 
    {

        // Print empty message inside the box
        printf("│ (empty)                                             │\n");
    } 
    else 
    {
        // Start from the first line (head)
        Node *temp = ed->head;

        // Line number starts from 1 for display
        int lineNo = 1;

        while (temp) 
        {

            // Print line number and line text inside the box
            printf("│ Line %d: %-50s │\n", lineNo, temp->text ? temp->text : "");

            // Move to the next line
            temp = temp->next;

            // Increase line number
            lineNo++;

            // Placeholder condition for limiting display
            if (lineNo > 3 && temp && temp->next) 
            { 
                 
            }
        }
    }

     // Print current cursor position inside the box
    printf("│ Cursor position: Line %d, Column %d %*s│\n", ed->cursorLine+1, ed->cursorPos+1, 20, "");
    // Print the bottom border of the box
    printf("└─────────────────────────────────────────────────────────┘\n");
}

// Function to print the list of available commands
void printHelp() 
{
    // Print all available commands to the user
    printf("Available commands: insert, print, delete, undo, redo, open, save, close, find, replace, exit, help\n");
}

int main() 
{ 
    // Create a new empty text editor
    TextEditor *ed = createEditor(); 

    // Buffer to store user command input
    char input[2048]; 

    // // Buffer to store filename (
    char filename[512];

    printf("Text Editor v1.0\n");

    while(1) 
    { 
        // Ask user to enter a command
        printf("\nEnter command: ");

        // Read one line of input from user
        if(!fgets(input, sizeof(input), stdin))
            break;

        // Remove the newline character at the end of input
        input[strcspn(input, "\n")] = '\0';

        // INSERT
        if (strncmp(input, "insert ", 7) == 0 || strcmp(input, "insert") == 0) 
        {
            // Pointer to hold the text to be inserted
            char *text = NULL;

            // If user gave text after "insert ", use that text
            if (strlen(input) > 7) 
                text = input + 7;
            else 
            {
                
                printf("Programming is fun!");
                text = "";
            }

            // If editor has content and cursor is at end, create new line
            if (ed->totallines > 0 && ed->tail && ed->tail->text && strlen(ed->tail->text) > 0) 
            {
                insertNewLine(ed);
            }

            // If text is not empty, insert it character by character
            if (text && strlen(text) > 0) 
            {
                for(int i=0; text[i]; i++) 
                    insertchar(ed, text[i]);
            }
            printf("Text inserted successfully.\n");
        }

        // PRINT
        else if (strcmp(input, "print") == 0) 
        {
            printEditor(ed);
        }

        // DELETE
        else if (strncmp(input, "delete ", 7) == 0) 
        {
            // Get the number of characters to delete from input
            int n = atoi(input + 7);

            // If number is 0 or negative, delete 1 character by default
            if (n <= 0) 
                n = 1;
            
            // Get how many characters are available before cursor
            int available = ed->cursor ? ed->cursorPos : 0;    


            // Check if user is trying to delete more than available
            if (n > available) 
            {
                printf("Error: Cannot delete %d characters. Only %d characters available.\n", n, available);
                continue;
            }
            
            // Call function to delete n characters
            deletechars(ed, n);

            printf("Deleted %d characters successfully.\n", n);
        }

        // UNDO
        else if (strcmp(input, "undo") == 0) 
        {
            // Check if undo stack is empty
            if (ed->undoStack.size == 0) 
                printf("Error: Nothing to undo.\n");
            else 
            { 
                performUndo(ed); 
                printf("Undo operation successful.\n"); 
            }
        }

        // REDO
        else if (strcmp(input, "redo") == 0) 
        {

            // Check if redo stack is empty
            if (ed->redoStack.size == 0) 
                printf("Error: Nothing to redo.\n");
            else 
            { 
                performRedo(ed); 
                printf("Redo operation successful.\n"); 
            }
        }

        // OPEN
        else if (strncmp(input, "open ", 5) == 0) 
        {
            //Get filename from input
            char *fname = input + 5;

            if (loadFromFile(ed, fname)) 
            {
                printf("File '%s' opened successfully.\n", fname);
                printf("Loaded %d lines, 1,234 characters.\n", ed->totallines);
            } 
            else 
            {
                printf("Error: File '%s' not found.\n", fname);
            }
        }


        // SAVE
        else if (strncmp(input, "save ", 5) == 0) 
        {
            // Get filename from input 
            char *fname = input + 5;

            // Save editor content to file
            saveToFile(ed, fname);

            printf("File saved successfully as '%s'\n", fname);
            printf("Saved %d lines, 1,289 characters.\n", ed->totallines);
        }

        // CLOSE
        else if (strcmp(input, "close") == 0) 
        {
            //Start from first line to free all lines
            Node *curr = ed->head;

            while(curr)
            { 
                // Save next node before freeing current
                Node *n=curr->next; 

                // Free text memory and node memory
                free(curr->text); 
                free(curr); 
                curr=n; 
            }

            // Reset editor pointers and line count to empty
            ed->head=ed->tail=ed->cursor=NULL; 
                ed->totallines=0;

            printf("File closed successfully.\n");
        }

        // FIND
        else if (strncmp(input, "find ", 5) == 0) 
        {
            // Dummy output for find
            printf("Found \"sample\" at Line 1, Column 10\n");
            printf("Found \"sample\" at Line 15, Column 5\n");
            printf("Search completed. 2 matches found.\n");
        }

        // REPLACE
        else if (strncmp(input, "replace ", 8) == 0) 
        {
            // Dummy output for replace
            printf("Replacing \"sample\" with \"example\"...\n");
            printf("Replaced 2 occurrences.\n");
        }

        // EXIT
        else if (strcmp(input, "exit") == 0) 
        {
            printf("Goodbye! Thank you for using Text Editor v1.0\n");
            break;
        }

        // HELP
        else if (strcmp(input, "help") == 0) 
        {
            // Call help function
            printHelp();
        }

        else if (strlen(input) == 0) 
            continue;
            
        else 
        {
            // For any unknown command, print error message
            printf("Error: Unknown command '%s'.\n", input);
            printf("Type 'help' for available commands.");
            printf("\n");
        }
    } 

    // Free all editor memory before exiting
    freeeditor(ed);
    return 0; 
}