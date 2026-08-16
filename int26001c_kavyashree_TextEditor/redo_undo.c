#include "redo_undo.h"

/* Initialize the dynamic array stack with the default capacity. */
void initializeStack(DynamicArrayStack *stack)
{
    stack->size = 0;
    stack->capacity = INITIAL_CAPACITY;

    stack->actions = malloc(sizeof(Action) * stack->capacity); 

    if (stack->actions == NULL)
    {
        stack->capacity = 0;
    }
}

/* Push an action onto the stack and resize the array when required. */
int push(DynamicArrayStack *stack, Action action)   
{
    if (stack->size == stack->capacity)
    {
        stack->capacity = stack->capacity * 2;      

        Action *temp = realloc(stack->actions,sizeof(Action) * stack->capacity);

        if (temp == NULL)
            return FAILURE;

        stack->actions = temp;
    }

    stack->actions[stack->size] = action;  
    stack->size++;

    return SUCCESS;
}


/* Remove and return the top action from the stack. */
Action pop(DynamicArrayStack *stack)   
{
    Action action;

    action.operation = NULL;
    action.text = NULL;
    action.cursorLine = 0;
    action.cursorPos = 0;

    if (stack->size == 0)
        return action;

    stack->size--;    

    action = stack->actions[stack->size];

    return action;
}


int isStackEmpty(DynamicArrayStack *stack)
{
    return stack->size == 0;
}

/* Free all actions and release the memory allocated for the stack. */
void clearStack(DynamicArrayStack *stack)      
{
    int i;

    if (stack == NULL)
        return;

    for (i = 0; i < stack->size; i++)
    {
        free(stack->actions[i].operation);
        free(stack->actions[i].text);
    }

    free(stack->actions);

    stack->actions = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

/* Create a deep copy of an action with separately allocated memory. */
Action copyAction(Action action)     
{
    Action newAction;

    newAction.operation = NULL;
    newAction.text = NULL;

    newAction.cursorLine = action.cursorLine;
    newAction.cursorPos = action.cursorPos;

    newAction.operation = malloc(strlen(action.operation) + 1);

    newAction.text = malloc(strlen(action.text) + 1);

    if (newAction.operation == NULL || newAction.text == NULL)
    {
        free(newAction.operation);
        free(newAction.text);

        newAction.operation = NULL;
        newAction.text = NULL;

        return newAction;
    }

    strcpy(newAction.operation,action.operation);

    strcpy(newAction.text,action.text);

    return newAction;
}

/* Create an action with the operation details and push it onto the stack. */
int recordAction(DynamicArrayStack *stack,const char *operation,const char *text,int cursorLine,
                 int cursorPos)
{
    Action action;        

    action.operation = malloc(strlen(operation) + 1);
    action.text = malloc(strlen(text) + 1);

    if (action.operation == NULL || action.text == NULL)
    {
        free(action.operation);
        free(action.text);

        return FAILURE;
    }

    strcpy(action.operation, operation);
    strcpy(action.text, text);

    action.cursorLine = cursorLine;
    action.cursorPos = cursorPos;

    return push(stack, action);     
}

/* Undo the most recent editing operation and move it to the redo stack. */
int undo(TextEditor *editor)      
{
    Action action;
    Action redoAction;

    if (editor == NULL)
        return FAILURE;

    if (isStackEmpty(&editor->undoStack))
    {
        printf("Error: Nothing to undo.\n");
        return FAILURE;
    }

    action = pop(&editor->undoStack);

    if (strcmp(action.operation, "insert_line") == 0)
    {
        editor->cursorLine = action.cursorLine;
        editor->cursorPos = action.cursorPos;

        Node *temp = editor->cursor;

    /* The inserted line is the current cursor line */
        if (temp != NULL)
        {
            if (temp->prev != NULL)
            {
                temp->prev->next = temp->next;
            }

            if (temp->next != NULL)
            {
                temp->next->prev = temp->prev;
            }

            if (temp == editor->tail)
            {
                editor->tail = temp->prev;
            }

            if (temp == editor->head)
            {
                editor->head = temp->next;
            }

            editor->cursor = temp->prev;

            free(temp->text);
            free(temp);

            editor->cursorLine--;

            if (editor->cursor != NULL)
            {
                editor->cursorPos = strlen(editor->cursor->text);
            }
            else
            {
                editor->cursorPos = 0;
            }
        }
    }
    else if (strcmp(action.operation, "insert") == 0)       
    {
        editor->cursorLine = action.cursorLine;
        editor->cursorPos = action.cursorPos;

        int len = strlen(editor->cursor->text);
        int textlen = strlen(action.text);

        char *newText = malloc(len - textlen + 1);

        if (newText == NULL)
            return FAILURE;

        strncpy(newText,editor->cursor->text,editor->cursorPos);

        strcpy(newText + editor->cursorPos,editor->cursor->text +editor->cursorPos +
               textlen);

        free(editor->cursor->text);
        editor->cursor->text = newText;
    }

    else if (strcmp(action.operation, "delete") == 0)   
    {
        editor->cursorLine = action.cursorLine;
        editor->cursorPos = action.cursorPos;

        int len = strlen(editor->cursor->text);
        int textlen = strlen(action.text);

        char *newText = malloc(len + textlen + 1);

        if (newText == NULL)
            return FAILURE;

        strncpy(newText,editor->cursor->text,editor->cursorPos);

        strcpy(newText + editor->cursorPos,action.text);

        strcpy(newText +editor->cursorPos +textlen,editor->cursor->text + editor->cursorPos);

        free(editor->cursor->text);

        editor->cursor->text = newText;
    }

    redoAction = copyAction(action);        

    if (redoAction.operation != NULL && redoAction.text != NULL)
    {
        push(&editor->redoStack, redoAction);
    }

    free(action.operation);     
    free(action.text);

    editor->modified = 1;

    printf("Undo operation successful.\n");

    return SUCCESS;
}

/* Redo the most recently undone operation and move it back to the undo stack. */
int redo(TextEditor *editor)
{
    Action action;
    Action undoAction;

    if (editor == NULL)
        return FAILURE;

    if (isStackEmpty(&editor->redoStack))
    {
        printf("Error: Nothing to redo.\n");
        return FAILURE;
    }

    action = pop(&editor->redoStack);
    
    if (strcmp(action.operation, "insert_line") == 0)
    {
        Node *newNode = createNode(action.text);

        if (newNode == NULL)
            return FAILURE;

        /*
        * After undo, cursor is on the previous line.
        * Insert the line after the current cursor.
        */
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

        editor->cursorLine = action.cursorLine;
        editor->cursorPos = strlen(action.text);
    }
    else if (strcmp(action.operation, "insert") == 0)
    {
        editor->cursorLine = action.cursorLine;
        editor->cursorPos = action.cursorPos;

        int len = strlen(editor->cursor->text);
        int textlen = strlen(action.text);

        char *newText = malloc(len + textlen + 1);

        if (newText == NULL)
            return FAILURE;

        strncpy(newText,editor->cursor->text,editor->cursorPos);

        strcpy(newText + editor->cursorPos,action.text);

        strcpy(newText +editor->cursorPos +textlen,editor->cursor->text + editor->cursorPos);

        free(editor->cursor->text);

        editor->cursor->text = newText;

        editor->cursorPos = action.cursorPos + textlen;
    }

    else if (strcmp(action.operation, "delete") == 0)
    {
        editor->cursorLine = action.cursorLine;
        editor->cursorPos = action.cursorPos;

        int len = strlen(editor->cursor->text);
        int textlen = strlen(action.text);

        char *newText = malloc(len - textlen + 1);

        if (newText == NULL)
            return FAILURE;

        strncpy(newText,editor->cursor->text,editor->cursorPos);

        strcpy(newText + editor->cursorPos,editor->cursor->text + editor->cursorPos + textlen);

        free(editor->cursor->text);

        editor->cursor->text = newText;
    }

    undoAction = copyAction(action);       

    if (undoAction.operation != NULL && undoAction.text != NULL)
    {
        push(&editor->undoStack, undoAction);
    }

    free(action.operation);
    free(action.text);

    editor->modified = 1;

    printf("Redo operation successful.\n");

    return SUCCESS;
}