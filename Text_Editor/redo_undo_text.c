#include "redo_undo_text.h"

#include "redo_undo_text.h"

void undo(Node **head, Node **tail, Stack *undoStack, Stack *redoStack)
{
    if(isempty(undoStack))
    {
        printf("Nothing to Undo\n");
        return;
    }

    Action *action = pop(undoStack);

    if(strcmp(action->operation, "insert") == 0)
    {
        char temp[MAX_LINE_LENGTH];

        delete_line(head,tail,action->line_no,temp);
    }

    else if(strcmp(action->operation, "delete") == 0)
    {
        insert_line(head,tail,action->text);
    }

    push(redoStack, *action);

    free(action);

    printf("Undo Successful\n");
}



void redo(Node **head,Node **tail,Stack *undoStack,Stack *redoStack)
{
    if(isempty(redoStack))
    {
        printf("Nothing to Redo\n");
        return;
    }

    Action *action = pop(redoStack);

    if(strcmp(action->operation,"insert")==0)
    {
        insert_line(head,tail,action->text);
    }

    else if(strcmp(action->operation,"delete")==0)
    {
        char temp[MAX_LINE_LENGTH];

        delete_line(head,tail,action->line_no,temp);
    }

    push(undoStack,*action);

    free(action);

    printf("Redo Successful\n");
}