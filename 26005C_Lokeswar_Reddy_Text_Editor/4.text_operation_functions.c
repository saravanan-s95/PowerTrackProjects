#include "header.h"

/*
Functions in this file:
    void insertText(TxtEdtr *editor, const char *text);
    void appendLine(TxtEdtr *editor, const char *text);
    void appendDeletedLine(TxtEdtr *editor, const char *text);
    Node *createNode(const char *text);

    void deleteCharacters(TxtEdtr *editor, int count);
    void deleteLine(TxtEdtr *editor);

    void copyText(TxtEdtr *editor)
    void cutText(TxtEdtr *editor)
    void pasteText(TxtEdtr *editor)

*/


char *clipboard = NULL;

void insertText(TxtEdtr *editor, const char *text)
{
    if (editor == NULL)
        return;
    
    /* If there is no current line, then add text in new line */
    if (editor->cursor == NULL) 
    {
        appendLine(editor, text);
        return;
    }

    Node *current       = editor->cursor;
    size_t oldLength    = strlen(current->txt); //length of "text in cursor pointing line"
    size_t insertLength = strlen(text);         //length of "to be inserted text"

    char *newText = malloc(oldLength + insertLength + 1);
    if (newText == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    memcpy(newText, current->txt, (size_t)editor->cpos);
    memcpy(newText + editor->cpos, text, insertLength);
    strcpy(newText + editor->cpos + insertLength, current->txt + editor->cpos);

    free(current->txt);
    current->txt = newText;
    editor->cpos += (int)insertLength;

    printf("Text inserted successfully.\n");
}


void appendLine(TxtEdtr *editor, const char *text)
{
    if (editor == NULL)
        return;

    /* New Node will be created and text copied to it */
    Node *newNode = createNode(text);
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    /* If there were no lines in editor, then below steps need to be performed */
    if (editor->head == NULL)
    {
        editor->head = newNode;
        editor->tail = newNode;
        editor->cursor = newNode;

        editor->cline = 1;
        editor->cpos = (int)strlen(newNode->txt);
        
        printf("Text inserted successfully.\n");
        return;
    }

    /* If there were lines already, then text should be appended in next line of cursor */
    newNode->nxt = editor->cursor ->nxt;
    newNode->prv = editor->cursor;
    
    if(editor->cursor->nxt != NULL)
        editor->cursor->nxt->prv= newNode;
    else
        editor->tail = newNode;
    
    editor->cursor->nxt= newNode;

    editor->cursor = newNode;

    editor->cline++;
    editor->cpos = (int)strlen(text);
    printf("Line inserted successfully\n");
}


void appendDeletedLine(TxtEdtr *editor, const char *text, int target_line)
{
    if (editor == NULL)
        return;

    /* New Node will be created and text copied to it */
    Node *newNode = createNode(text);
    if (newNode == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    /* If there were no lines in editor, then below steps need to be performed */
    if (editor->head == NULL)
    {
        editor->head = newNode;
        editor->tail = newNode;
        editor->cursor = newNode;

        editor->cline = 1;
        editor->cpos = (int)strlen(newNode->txt);
        
        printf("Text inserted successfully.\n");
        return;
    }

    /*  case 1: If Line should be created at Line1 place     
                Inserting back (deleted or cut) Line at Line1 */
    if( editor->cline == 1 && target_line == 1 )
    {
        printf("case 1\n");
        editor->head = newNode;
        newNode->prv = NULL;

        newNode->nxt = editor->cursor;
        editor->cursor->prv = newNode;
        editor->cursor = newNode;
        editor->cpos = (int)strlen(newNode->txt);
        return;
    }

    /* case 2: Cursor will be in previous line 
               And new node should be inserted after current pointing line */
    if( target_line == editor->cline+1 )
    {
        printf("case 2\n");
        if( editor->cursor->nxt != NULL )
            editor->cursor->nxt->prv = newNode;
        else
            editor->tail = newNode;

        newNode->nxt = editor->cursor->nxt;

        editor->cursor->nxt = newNode;
        
        newNode->prv = editor->cursor;

        editor->cursor = newNode;
        editor->cline++;

        editor->cpos = (int)strlen(newNode->txt);
        return;
    }

    /* case 3: Cursor will be in currect position 
               And new node should be inserted at current pointing line  */
    
    newNode->nxt = editor->cursor;
    newNode->prv = editor->cursor->prv;
    
    editor->cursor->prv->nxt = newNode;
    editor->cursor->prv = newNode;
    editor->cursor = newNode;

    editor->cpos = (int)strlen(text);
    printf("Line inserted successfully.\n");
}


Node *createNode(const char *text)
{
    if (text == NULL)
        text = "";

    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
        return NULL;

    newNode->txt = malloc(strlen(text) + 1);
    if (newNode->txt == NULL)
    {
        free(newNode);
        return NULL;
    }

    strcpy(newNode->txt, text);
    newNode->prv = NULL;
    newNode->nxt = NULL;

    return newNode;
}


void deleteCharacters(TxtEdtr *editor, int count)
{
    Node *current = editor->cursor;
    int length = (int)strlen(current->txt);

    memmove(current->txt + editor->cpos - count, current->txt + editor->cpos, (size_t)(length - editor->cpos + 1));

    editor->cpos -= count;
    
    if((editor->cursor == editor->tail) && (editor->cursor == editor->head) && (editor->cpos==0))
        initEditor(editor);
    
    printf("Deleted %d characters successfully.\n", count);
}


void deleteLine(TxtEdtr *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        printf("Error: Editor is empty.\n");
        return;
    }

    Node *current  = editor->cursor;
    Node *previous = current->prv;
    Node *next     = current->nxt;

    if (previous != NULL)
        previous->nxt = next;
    else
        editor->head = next;
    

    if (next != NULL)
        next->prv = previous;
    else
        editor->tail = previous;
    

    free(current->txt);
    free(current);


    /* If next line exist, then cursor should point to it */
    if (next != NULL)
    {
        editor->cursor = next;
        editor->cpos = 0;
    }

    /* If next line not exist, and previous line exists, 
       then cursor should point to previous line*/
    else if (previous != NULL)
    {
        editor->cursor = previous;
        editor->cline--;
        editor->cpos = (int)strlen(editor->cursor->txt);
        
    }

    /* If both next and previous lines are not exists, 
       then cursor should point to NULL, means editor is empty*/
    else
    {
        editor->cursor = NULL;
        editor->cline = 1;
        editor->cpos = 0;
        return;
    }

    
}


void copyText(TxtEdtr *editor)
{
    
    if (editor == NULL || editor->cursor == NULL)
    {
        printf("Error: Editor is empty.\n");
        return;
    }
  
    free(clipboard);
    
    clipboard = malloc(strlen(editor->cursor->txt) + 1);
    if (clipboard == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(clipboard, editor->cursor->txt);
    printf("Text copied successfully.\n");
}


void cutText(TxtEdtr *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        printf("Error: Editor is empty.\n");
        return;
    }

    copyText(editor);
    deleteLine(editor);
    printf("Text cut successfully.\n");
}


void pasteText(TxtEdtr *editor)
{
    if (editor == NULL)
        return;
    
    if (clipboard == NULL)
    {
        printf("Clipboard is empty.\n");
        return;
    }
    insertText(editor, clipboard);

    printf("Text pasted successfully.\n");
}
