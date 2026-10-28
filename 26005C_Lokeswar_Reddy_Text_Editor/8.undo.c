#include "header.h"
#include<unistd.h>

/*
Functions in this file:
    void undo(TxtEdtr *editor,DynArrStk *undoStack,DynArrStk *redoStack);
    void removeInsertedTextAt(TxtEdtr *editor, int line, int pos, const char *text)
    Node *getLineNode(TxtEdtr *editor, int lineNumber)
    int setCursor(TxtEdtr *editor, int line, int pos)
*/


void undo(TxtEdtr *editor,DynArrStk *undoStack,DynArrStk *redoStack)
{
    Action action;

    if (!popStack(undoStack, &action))
    {
        printf("Error: Nothing to undo.\n");
        return;
    }
    
    printf("last action from stack:%s\n",action.opr);

    if (strcmp(action.opr, "insert") == 0)
    {
        removeInsertedTextAt(editor,action.cline,action.cpos,action.text);
        if(action.cline == 1 && action.cpos == 0)
            initEditor(editor);
    }
    
    else if (strcmp(action.opr, "insert_new_line") == 0)
    {
        if (setCursor(editor, action.cline, 0))
            deleteLine(editor);
        if(action.cline == 1 && action.cpos == 0)
            initEditor(editor);
    }

    else if (strcmp(action.opr, "delete") == 0)
    {
        if (!setCursor(editor, action.cline,action.cpos))            
            return;
        
        insertText(editor, action.text);
    }

    else if (strcmp(action.opr, "delete_line") == 0)
    {
        if(action.text == NULL)
        {
            printf("No text, so creating node\n");

            Node *newNode = malloc(sizeof(Node));
            if (newNode == NULL)
                return;

            newNode->txt = calloc(1,1);

            Node *PrevNode = getLineNode(editor, action.cline-1);

            newNode->prv = PrevNode;
            newNode->nxt = PrevNode->nxt;

            if(PrevNode->nxt != NULL)
                PrevNode->nxt->prv= newNode;
            else
                editor->tail = newNode;
            
            PrevNode->nxt= newNode;

            editor->cursor = newNode;

            editor->cline = action.cline;
            editor->cpos = 0;
            printf("Line inserted successfully.\n");

        }
        else
        {
            if (!setCursor(editor, action.cline,action.cpos))            
                return;
            
            appendDeletedLine(editor, action.text, action.cline);
        }
    }

    else if (strcmp(action.opr, "cut") == 0)
    {
        if (!setCursor(editor, action.cline,action.cpos))            
            return;
        
        appendDeletedLine(editor, action.text,action.cline);
    }
    else if (strcmp(action.opr, "paste") == 0)
    {
        // if (!setCursor(editor, action.cline,editor->cpos))            
        //     return;
        
        deleteCharacters(editor, strlen(action.text));
    }
    
    pushStack(redoStack, action.opr, action.text,action.cline, action.cpos);

    free(action.opr);
    free(action.text);
    
    printf("Undo operation successful.\n");
}


void removeInsertedTextAt(TxtEdtr *editor, int line, int pos, const char *text)
{
    Node *node = getLineNode(editor, line);

    if (node == NULL)      // || text == NULL
        return;
    
    size_t action_text_length = strlen(text);
    size_t total_line_length  = strlen(node->txt);

    if (pos < 0 || (size_t)pos > total_line_length || action_text_length > total_line_length - (size_t)pos){
        return;
    }

    if (strncmp(node->txt + pos, text, action_text_length) != 0){
        return;
    }

    memmove(node->txt + pos,node->txt + pos + action_text_length, total_line_length - (size_t)pos - action_text_length + 1);

    editor->cursor = node;
    editor->cline = line;
    editor->cpos = pos;
}

Node *getLineNode(TxtEdtr *editor, int lineNumber)
{
    Node *current;
    int line = 1;
    if (editor == NULL || lineNumber < 1)
        return NULL;
    
        
    current = editor->head;
    while (current != NULL && line < lineNumber)
    {
        current = current->nxt;
        line++;
    }
    return (line == lineNumber) ? current : NULL;
}


int setCursor(TxtEdtr *editor, int line, int pos)
{   
    /* If editor is empty, new line will be inserted by appendLine() later */
    if(editor->cursor == NULL)
        return 1;

    /* If cursor is at currect position, no need of finding node again */
    if( line == editor->cline+1 )
        return 1;

    Node *node = getLineNode(editor, line);
    if (node == NULL)
        return 0;

    editor->cursor = node;
    editor->cline = line;
    editor->cpos = pos;
    return 1;
}