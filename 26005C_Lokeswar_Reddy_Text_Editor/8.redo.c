#include "header.h"


void redo(TxtEdtr *editor, DynArrStk *undoStack, DynArrStk *redoStack)
{
    Action action;

    if (!popStack(redoStack, &action))
    {
        printf("Error: Nothing to redo.\n");
        return;
    }

    if (strcmp(action.opr, "insert") == 0)
    {
        if (!setCursor(editor, action.cline,action.cpos))
            return;
        
        insertText(editor, action.text);
    }
    else if (strcmp(action.opr, "insert_new_line") == 0)
    {
        Node *node    = getLineNode(editor, action.cline - 1);
        if (node == NULL)
        {
            appendLine(editor, action.text);
        }
        else
        {
            Node *newNode = createNode(action.text);
            if (newNode == NULL) 
            {
                printf("Memory Allocation Failed\n");
                return;
            }
            else
            {
                newNode->prv = node;
                newNode->nxt = node->nxt;

                node->nxt = newNode;
                if (node->nxt != NULL)
                    node->nxt->prv = newNode;
                else
                    editor->tail = newNode;
                
                editor->cursor = newNode;
                editor->cline = action.cline;
                editor->cpos = (int)strlen(newNode->txt);
            }
        }   
    }

    else if (strcmp(action.opr, "delete") == 0)
    {
        if (setCursor(editor,action.cline,action.cpos + (int)strlen(action.text)))
            deleteCharacters(editor, (int)strlen(action.text));
    } 
    else if (strcmp(action.opr, "delete_line") == 0)
    {
        if (setCursor(editor,action.cline,action.cpos))
            deleteLine(editor);
    }

    else if (strcmp(action.opr, "cut") == 0)
    {
        deleteLine(editor);
    }
    else if (strcmp(action.opr, "paste") == 0)
    {
        deleteCharacters(editor, strlen(action.text));
    }

    

    pushStack(undoStack,action.opr, action.text,action.cline, action.cpos);

    free(action.opr);
    free(action.text);
    printf("Redo operation successful.\n");
}





