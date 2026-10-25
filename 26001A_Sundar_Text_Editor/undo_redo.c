#include "text_editor.h"
#include "undo_redo.h"

extern int lineCount;

int initialize_Undo_Redo_Stack(DynamicArrayStack* undoStack,DynamicArrayStack* redoStack)
{
    /* Allocate memory for undo stack */
    undoStack->actions = malloc(STACK_CAPACITY * sizeof(Action));
    if(undoStack->actions == NULL)
    {
        printf("Memory not allocated\n");
        return FAILURE;
    }

    /* Allocate memory for redo stack */
    redoStack->actions = malloc(STACK_CAPACITY * sizeof(Action));
    if(redoStack->actions == NULL)
    {
        printf("Memory not allocated\n");
        free(undoStack->actions);
        undoStack->actions = NULL;
        return FAILURE;
    }

    /* Set capacity and size for undo and redo */
    undoStack->capacity = STACK_CAPACITY;
    redoStack->capacity = STACK_CAPACITY;
    undoStack->size = -1;
    redoStack->size = -1;

    return SUCCESS;
}

int undo_operation(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack)
{
    /* If undo stack is empty */
    if(undoStack->size == -1)
    {
        printf("There is no previous operation performed.Undo failed\n");
        return FAILURE;
    }

    /* Take temp Action* for the current undo action */
    Action* undoAction = &undoStack->actions[undoStack->size];

    /* Copy the action to redo before changing/freeing the undo action */
    ++redoStack->size;
    if(redoStack->size >= redoStack->capacity)
    {
        --redoStack->size;
        printf("Redo capacity has reached maximum\n");
        return FAILURE;
    }

    /* Take temp Action* for the current storing of redo action */
    Action* redoAction = &redoStack->actions[redoStack->size];

    /* Copy undo actions to redo */
    redoAction->operation = undoAction->operation;
    redoAction->text = malloc(strlen(undoAction->text) + 1);
    strcpy(redoAction->text,undoAction->text);
    redoAction->cursorLine = undoAction->cursorLine;
    redoAction->cursorPos = undoAction->cursorPos;

    /* Locate the line on which the original operation happened. */
    Node* temp = editor->head;
    int i = 1;
    while(i != undoAction->cursorLine && temp)
    {
        temp = temp->next;
        i++;
    }

    editor->current = temp;
    editor->cursorLine = undoAction->cursorLine;
    editor->cursorPos = undoAction->cursorPos;

    /* Set logAction to 1 to prevent logging of current undo operation */
    extern int logAction;
    logAction = 1;

    /* Assign failure to result to check whether undo is successful or not at the end */
    int result = FAILURE;

    /* Perform inverse action */
    if(strcmp(undoAction->operation,"insert_line") == 0)
    {    
        if(deleteLine(editor,undoStack) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(undoAction->operation,"insert_chars") == 0)
    {
        int len = strlen(undoAction->text);
        if(deleteLeft(editor,undoStack,len) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(undoAction->operation,"delete_line") == 0)
    {   /* If deleted line is first line */
        if(undoAction->cursorLine == 1)
        {
            editor->current = NULL;
            editor->cursorLine = 0;
            editor->cursorPos = 0;
        }
        else
        {
            /* If last line was deleted */
            if(temp == NULL)
            {
                editor->current = editor->tail;
                editor->cursorLine = lineCount;
            }
            else
            {
                editor->current = temp->prev;
                editor->cursorLine = undoAction->cursorLine - 1;
            }
            editor->cursorPos = editor->current ? strlen(editor->current->line) : 0;
        }

        if(insertTextnewline(editor,undoStack,undoAction->text) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(undoAction->operation,"delete_chars") == 0)
    {
        if(insertText(editor,undoStack,undoAction->text) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    /* Set logAction to 0 to log future undo operations */
    logAction = 0;

    /* If success free the undo stack top most node */
    if(result == SUCCESS)
    {
        free(undoAction->text);
        undoAction->text = NULL;
        --undoStack->size;
        return SUCCESS;
    }

    /* The undo action is still valid, so remove the temporary redo action. */
    free(redoAction->text);
    redoAction->text = NULL;
    --redoStack->size;
    return FAILURE;
}

int redo_operation(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack)
{
    /* If redo stack is empty */
    if(redoStack->size == -1)
    {
        printf("There is no previous undo operation performed.Redo failed\n");
        return FAILURE;
    }

    /* Take temp Action* for the current storing of redo action */
    Action* redoAction = &redoStack->actions[redoStack->size];

    /* Store the current logAction and set to 0 for logging undo */
    extern int logAction;
    int oldLogAction = logAction;
    logAction = 0;

    /* Assign failure to result to check whether redo is successful or not at the end */
    int result = FAILURE;

    /* Locate the line on which the original operation happened. */
    Node* temp = editor->head;
    int i = 1;
    while(i != redoAction->cursorLine && temp)
    {
        temp = temp->next;
        i++;
    }

    editor->current = temp;
    editor->cursorLine = redoAction->cursorLine;
    editor->cursorPos = redoAction->cursorPos;    

    if(strcmp(redoAction->operation,"insert_line") == 0)
    {
        /* The stored line is the line that must be recreated. */
        if(redoAction->cursorLine == 1)
        {
            editor->current = NULL;
            editor->cursorLine = 0;
            editor->cursorPos = 0;
        }
        else
        {
            /* If last line was deleted */   
            if(temp == NULL)
            {
                editor->current = editor->tail;
                editor->cursorLine = lineCount;
            }
            else
            {
                editor->current = temp;
                editor->cursorLine = redoAction->cursorLine - 1;
            }
            editor->cursorPos = editor->current ? strlen(editor->current->line) : 0;
        }

        if(insertTextnewline(editor,undoStack,redoAction->text) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(redoAction->operation,"insert_chars") == 0)
    {
        editor->cursorPos = redoAction->cursorPos - strlen(redoAction->text);
        if(insertText(editor,undoStack,redoAction->text) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(redoAction->operation,"delete_line") == 0)
    {
        /* If last line was deleted */
        if(temp == NULL)
        {
            editor->current = editor->tail;
            editor->cursorLine = lineCount;
        }
        else
        {
            editor->current = temp;
            editor->cursorLine = redoAction->cursorLine;
        }
        editor->cursorPos = editor->current ? strlen(temp->line) : 0;

        if(deleteLine(editor,undoStack) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    else if(strcmp(redoAction->operation,"delete_chars") == 0)
    {
        editor->cursorPos = redoAction->cursorPos + strlen(redoAction->text);
        if(deleteLeft(editor,undoStack,strlen(redoAction->text)) == SUCCESS)
        {
            result = SUCCESS;
        }
    }

    /* Reset logAction for logging future undo operations */
    logAction = oldLogAction;

    if(result == SUCCESS)
    {
        /* The normal operation created the corresponding undo action. */
        free(redoAction->text);
        redoAction->text = NULL;
        --redoStack->size;
        return SUCCESS;
    }
    return FAILURE;
}

void reset_redo(DynamicArrayStack* redoStack)
{
    /* Free text in actions array and decrement the redo stack size until -1 */
    while(redoStack->size > -1)
    {
        free(redoStack->actions[redoStack->size].text);
        redoStack->actions[redoStack->size].text = NULL;
        --redoStack->size;
    }
}

void delete_text_editor(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack)
{
    /* Reset redo stack */
    reset_redo(redoStack);

    /* Free text in actions array and decrement the undo stack size until -1 */
    while(undoStack->size > -1)
    {
        free(undoStack->actions[undoStack->size].text);
        undoStack->actions[undoStack->size].text = NULL;
        --undoStack->size;
    }

    /* Free the entire undo stack actions array */
    free(undoStack->actions);
    undoStack->actions = NULL;

    /* Free the entire redo stack actions array */
    free(redoStack->actions);
    redoStack->actions = NULL;

    /* Free text editor */
    while(editor->head)
    {
        editor->current = editor->head;
        editor->head = editor->current->next;
        free(editor->current);
    }

    editor->current = NULL;
    editor->tail = NULL;
}
