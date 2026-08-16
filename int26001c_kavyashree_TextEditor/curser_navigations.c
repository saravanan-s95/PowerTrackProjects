#include "curser_navigations.h"

/* Move the cursor to the previous line and adjust its position if required. */
void moveCursorUp(TextEditor *editor)      
{
    if (editor == NULL || editor->cursor == NULL)   
    {
        return;
    }

    if (editor->cursor->prev == NULL)          
    {
        printf("Already at first line.\n");
        return;
    }

    editor->cursor = editor->cursor->prev;   

    editor->cursorLine--;         

    if (editor->cursorPos > strlen(editor->cursor->text))         
    {
        editor->cursorPos = strlen(editor->cursor->text);           
    }
}

/* Move the cursor to the next line and adjust its position if required. */
void moveCursorDown(TextEditor *editor)
{
    if (editor == NULL ||
        editor->cursor == NULL)
    {
        return;
    }

    if (editor->cursor->next == NULL)
    {
        printf("Already at last line.\n");
        return;
    }

    editor->cursor = editor->cursor->next;

    editor->cursorLine++;

    if (editor->cursorPos > strlen(editor->cursor->text))      
    {
        editor->cursorPos = strlen(editor->cursor->text);
    }
}

/* Move the cursor one character position to the left within the current line. */
void moveCursorLeft(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return;
    }

    if (editor->cursorPos > 0)             
    {
        editor->cursorPos--;                
    }
    else
    {
        printf("Already at start of line.\n"); }
}

/* Move the cursor one character position to the right within the current line. */
void moveCursorRight(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return;
    }

    int length = strlen(editor->cursor->text); 
    if (editor->cursorPos < length)        
    {
        editor->cursorPos++;               
     }
    else
    {
        printf("Already at end of line.\n");    
    }
}

/* Move the cursor to the beginning of the current line. */
void jumpToStartOfLine(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return;
    }

    editor->cursorPos = 0;         
}

/* Move the cursor to the end of the current line. */
void jumpToEndOfLine(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return;
    }
   
    editor->cursorPos = strlen(editor->cursor->text);  
}

/* Move the cursor to the beginning of the file. */
void jumpToStartOfFile(TextEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->head == NULL)
    {
        return;
    }

    editor->cursor = editor->head;     

    editor->cursorLine = 1;

    editor->cursorPos = 0;
}

/* Move the cursor to the end of the file and position it at the end of the last line. */
void jumpToEndOfFile(TextEditor *editor)
{
    if (editor == NULL)
    {
        return;
    }

    if (editor->tail == NULL)
    {
        return;
    }

    editor->cursor = editor->tail;

    editor->cursorLine = 1;

    Node *temp = editor->head;
 
    while (temp != editor->tail)       
    {
        editor->cursorLine++;
        temp = temp->next;
    }

    editor->cursorPos =strlen(editor->cursor->text);   
}