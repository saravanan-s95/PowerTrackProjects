#include "text_ops.h"
#include "redo_undo.h"

/* Insert text at the current cursor position or create a new line when at the line end. */
int insertText(TextEditor *editor, const char *text)
{
     
    if (editor->head == NULL)
    {
        Node *newNode = createNode(text);

        if (newNode == NULL)
            return FAILURE;

        editor->head = newNode;
        editor->tail = newNode;
        editor->cursor = newNode;

        editor->cursorLine = 1;
        editor->cursorPos = strlen(text);

        editor->modified = 1;

        recordAction(&editor->undoStack,"insert",text,editor->cursorLine,0);

        clearStack(&editor->redoStack);

        printf("Text inserted successfully.\n");

        return SUCCESS;
    }

    /*
     * If cursor is at the end of the current line,
     * create a new line.
     */
    if (editor->cursorPos == strlen(editor->cursor->text))
    {
        Node *newNode = createNode(text);

        if (newNode == NULL)
            return FAILURE;

        newNode->prev = editor->cursor;
        newNode->next = editor->cursor->next;

        if (editor->cursor->next != NULL)
        {
            editor->cursor->next->prev = newNode;
        }
        else
        {
            editor->tail = newNode;
        }

        editor->cursor->next = newNode;

        editor->cursor = newNode;

        editor->cursorLine++;
        editor->cursorPos = strlen(text);

        editor->modified = 1;

        recordAction(&editor->undoStack,"insert_line",text,editor->cursorLine,0);

        clearStack(&editor->redoStack);

        printf("Text inserted successfully.\n");

        return SUCCESS;
    }

     

    if (editor == NULL || editor->cursor == NULL || text == NULL)  
    {
        return FAILURE;
    }

    int insertlen = strlen(text);     

    if (insertlen == 0)        
    {
        return FAILURE;
    }
  
    int oldlen = strlen(editor->cursor->text);        
 
    char *newtext = malloc(oldlen + insertlen + 1);    

    if (newtext == NULL)             
    {
        return FAILURE;
    }

    strncpy(newtext, editor->cursor->text, editor->cursorPos);      
    newtext[editor->cursorPos] = '\0';
    strcpy(newtext + editor->cursorPos, text);         

    strcpy(newtext + editor->cursorPos + insertlen, editor->cursor->text + editor->cursorPos);    

    free(editor->cursor->text);         

    editor->cursor->text = newtext;      

    editor->cursorPos = editor->cursorPos + insertlen;         

    editor->modified = 1;           

     
    recordAction(&editor->undoStack,"insert",text,editor->cursorLine,editor->cursorPos - insertlen);  

    clearStack(&editor->redoStack);           

    printf("Text inserted successfully.\n");

    return SUCCESS;
}

/* Delete a specified number of characters from the current cursor position. */
int deleteCharacters(TextEditor *editor, int count)
{
     

    if (editor == NULL || editor->cursor == NULL)
    {
        return FAILURE;
    }

    if (count <= 0)         
    {
        return FAILURE;
    }

    int length = strlen(editor->cursor->text);       

    int available = length - editor->cursorPos;       
 
    if (count > available)                       
    {
        printf("Error: Cannot delete %d characters. Only %d characters available.\n",count,available);
        return FAILURE;
    }


    int cursorLine = editor->cursorLine;        
    int cursorPos = editor->cursorPos;

    char *deletedtext = malloc(count + 1);       

    if (deletedtext == NULL)
    {
        return FAILURE;
    }

    strncpy(deletedtext,editor->cursor->text + editor->cursorPos,count);   

    deletedtext[count] = '\0';     

    char *newtext = malloc(length - count + 1);    

    if (newtext == NULL)
    {
        free(deletedtext);
        return FAILURE;
    }

    strncpy(newtext,editor->cursor->text,editor->cursorPos);   

    strcpy(newtext + editor->cursorPos, editor->cursor->text + editor->cursorPos + count);   

    free(editor->cursor->text);      

    editor->cursor->text = newtext;   

    editor->modified = 1;          

    recordAction(&editor->undoStack,"delete",deletedtext,cursorLine,cursorPos);     

    free(deletedtext);   

    clearStack(&editor->redoStack);

    printf("Deleted %d characters successfully.\n", count);

    return SUCCESS;
}

/* Copy a specified number of characters from the cursor position to the clipboard. */
int copyText(TextEditor *editor, int count)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return FAILURE;
    }

    if (count <= 0)
    {
         return FAILURE;
    }

    int length = strlen(editor->cursor->text);

    int available = length - editor->cursorPos;

    if (count > available)          
    {
        printf("Error: Cannot copy %d characters.Only %d characters available.\n", count, available);

        return FAILURE;
    }

    free(editor->clipboard);         

    editor->clipboard = malloc(count + 1);         

    if (editor->clipboard == NULL)
    {
        return FAILURE;
    }

    strncpy(editor->clipboard,editor->cursor->text + editor->cursorPos,count);     

    editor->clipboard[count] = '\0';                         

    printf("Copied %d characters successfully.\n", count);

    return SUCCESS;
}

/* Copy the specified text to the clipboard and then remove it from the editor. */
int cutText(TextEditor *editor, int count)         
{
    if (editor == NULL)            
    {
        return FAILURE;
    }

    if (copyText(editor, count) == FAILURE)       
    {
        return FAILURE;
    }

    return deleteCharacters(editor, count);        
}

/* Insert the contents of the clipboard at the current cursor position. */
int pasteText(TextEditor *editor)
{
    if (editor == NULL || editor->clipboard == NULL)     
    {
        printf("Clipboard is empty.\n");
        return FAILURE;
    }

    return insertText(editor, editor->clipboard);     
}

/* Delete the entire line containing the current cursor and update the cursor position. */
int deleteLine(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        return FAILURE;
    }

    Node *temp = editor->cursor;    

    if (temp == editor->head && temp == editor->tail)        
    {
        free(temp->text);
        free(temp);

        editor->head = NULL;         
        editor->tail = NULL;
        editor->cursor = NULL;

        editor->cursorLine = 1;
        editor->cursorPos = 0;

        editor->modified = 1;    

        return SUCCESS;
    }
 
    if (temp == editor->head)             
    {
        editor->head = temp->next;           
        editor->head->prev = NULL;

        editor->cursor = editor->head;      

        free(temp->text);
        free(temp);

        editor->cursorLine = 1;       
        editor->cursorPos = 0;

        editor->modified = 1;

        return SUCCESS;
    }

    if (temp == editor->tail)      
    {
        editor->tail = temp->prev;       
        editor->tail->next = NULL;

        editor->cursor = editor->tail;    

        free(temp->text);
        free(temp);

        editor->cursorLine--;          

        if (editor->cursorPos > strlen(editor->cursor->text))       
        {
            editor->cursorPos = strlen(editor->cursor->text);        
        }

        editor->modified = 1;

        return SUCCESS;
    }

     
    temp->prev->next = temp->next;        
    temp->next->prev = temp->prev;

    editor->cursor = temp->next;     

    free(temp->text);
    free(temp);

    editor->cursorPos = 0;

    editor->modified = 1;

    return SUCCESS;
}