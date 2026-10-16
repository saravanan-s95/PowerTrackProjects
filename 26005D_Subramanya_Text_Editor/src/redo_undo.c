#include "redo_undo.h"

Status_e push_st(DynamicArrayStack *stack, Action newAction)
{
    /* Skip push if called internally by undo/redo with no target stack */
    if(stack == NULL)
        return FAILURE;

    /* If stack is full increase the capacity */
    if(stack->size == stack->capacity)
    {
        /* Doubles the size of stack array */
        Action *temp;
        int new_capacity = (stack->capacity == 0) ? 1 : stack->capacity * 2;
        temp = realloc(stack->actions,
                        new_capacity * sizeof(Action));

        if(temp == NULL)
        {
            fprintf(stderr, "ERROR: Could not allocate memory for stack array!\n");
            return FAILURE;
        }

        stack->actions = temp;
        stack->capacity = new_capacity;
    }

    /* Push the newAction to the stack  */
    stack->actions[stack->size] = newAction;
    stack->size++;

    return SUCCESS;

}

Action pop_st(DynamicArrayStack *stack)
{
    if(stack->size > 0)
    {
        stack->size--;
    }
    else
        return (Action){0};

    /* Return the popped action for redo/undo */
    return stack->actions[stack->size];
}

Status_e undo(TextEditor *editor, DynamicArrayStack *undoStack, DynamicArrayStack *redoStack)
{
    if(undoStack->size == 0)
    {
        printf("Nothing left to undo!\n");
        return FAILURE;
    }

    /* Pop the undoSTack and perform REVERSE action */
    Action oldAction = pop_st(undoStack);
    if(oldAction.text == NULL)
    {
        printf("DEBUG ERROR: POP has failed!\n");
        return FAILURE;
    }

    if(oldAction.operation == 'I')
    {
        /* Call deletecharacter */
        deleteCharacters(editor, oldAction.cursorLine, oldAction.cursorPos + strlen(oldAction.text), strlen(oldAction.text), NULL);
    }
    else if(oldAction.operation == 'D')
    {
        /* Call insertText*/
        insertText(editor, oldAction.cursorLine, oldAction.cursorPos, oldAction.text, NULL);
    }

    push_st(redoStack, oldAction);
    return SUCCESS;

}


Status_e redo(TextEditor *editor, DynamicArrayStack *undoStack, DynamicArrayStack *redoStack)
{
    if(redoStack->size == 0)
    {
        printf("Nothing left to redo!\n");
        return FAILURE;
    }

    /* Pop the redoSTack and perform REPLAY action */
    Action oldAction = pop_st(redoStack);

    if(oldAction.operation == 'I')
    {
        /* Call insertText */
        insertText(editor, oldAction.cursorLine, oldAction.cursorPos, oldAction.text, NULL);
    }
    else if(oldAction.operation == 'D')
    {
        /* Call deleteCharacters*/
        deleteCharacters(editor, oldAction.cursorLine, oldAction.cursorPos + strlen(oldAction.text), strlen(oldAction.text), NULL);
    }

    push_st(undoStack, oldAction);
    return SUCCESS;
}