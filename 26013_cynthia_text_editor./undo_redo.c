
#include <stdlib.h>
#include <string.h>

#include "undo_redo.h"
#include "text_ops.h"

int init_stack(Stack *stack)
{
    if (stack == NULL)
        return -1;

    stack->capacity = 4;
    stack->top = -1;

    stack->array = malloc(stack->capacity * sizeof(EditOperation));

    if (stack->array == NULL)
        return -1;

    return 0;
}

int push(Stack *stack, EditOperation operation)
{
    EditOperation *temp;

    if (stack == NULL || stack->array == NULL)
        return -1;

    if (stack->top == stack->capacity - 1)
    {
        temp = realloc(stack->array,
                       stack->capacity * 2 * sizeof(EditOperation));

        if (temp == NULL)
            return -1;

        stack->array = temp;
        stack->capacity *= 2;
    }

    stack->top++;
    stack->array[stack->top] = operation;

    return 0;
}

int is_empty(Stack *stack)
{
    if (stack == NULL || stack->top == -1)
        return 1;

    return 0;
}

int pop(Stack *stack, EditOperation *operation)
{
    if (stack == NULL || operation == NULL || is_empty(stack))
        return -1;

    *operation = stack->array[stack->top];
    stack->top--;

    return 0;
}

int record_operation(Stack *stack, int operation,
                     int line_number, int position, char character)
{
    EditOperation op;

    op.operation = operation;
    op.line_number = line_number;
    op.position = position;
    op.character = character;

    return push(stack, op);
}

int undo_operation(Stack *undo_stack, Stack *redo_stack, Node *head)
{
    EditOperation op;
    Node *current;
    int result;

    if (undo_stack == NULL || redo_stack == NULL ||
        is_empty(undo_stack))
        return -1;

    op = undo_stack->array[undo_stack->top];

    current = get_node(head, op.line_number);

    if (current == NULL)
        return -1;

    if (op.operation == INSERT_CHAR)
    {
        result = delete_char(current, op.position);
    }
    else if (op.operation == DELETE_CHAR)
    {
        result = insert_char(current, op.position, op.character);
    }
    else
    {
        return -1;
    }

    if (result != 0)
        return -1;

    if (push(redo_stack, op) != 0)
    {
        /* Restore the edit if saving for Redo fails. */
        if (op.operation == INSERT_CHAR)
            insert_char(current, op.position, op.character);
        else
            delete_char(current, op.position);

        return -1;
    }

    pop(undo_stack, &op);

    return 0;
}

int redo_operation(Stack *undo_stack, Stack *redo_stack, Node *head)
{
    EditOperation op;
    Node *current;
    int result;

    if (undo_stack == NULL || redo_stack == NULL ||
        is_empty(redo_stack))
        return -1;

    op = redo_stack->array[redo_stack->top];

    current = get_node(head, op.line_number);

    if (current == NULL)
        return -1;

    if (op.operation == INSERT_CHAR)
    {
        result = insert_char(current, op.position, op.character);
    }
    else if (op.operation == DELETE_CHAR)
    {
        result = delete_char(current, op.position);
    }
    else
    {
        return -1;
    }

    if (result != 0)
        return -1;

    if (push(undo_stack, op) != 0)
    {
        /* Reverse the change if saving for Undo fails. */
        if (op.operation == INSERT_CHAR)
            delete_char(current, op.position);
        else
            insert_char(current, op.position, op.character);

        return -1;
    }

    pop(redo_stack, &op);

    return 0;
}

int insert_char_with_undo(Node *cursor, int line_number,
                          int position, char character,
                          Stack *undo_stack, Stack *redo_stack)
{
    if (cursor == NULL || undo_stack == NULL || redo_stack == NULL)
        return -1;

    if (insert_char(cursor, position, character) != 0)
        return -1;

    if (record_operation(undo_stack, INSERT_CHAR,
                         line_number, position, character) != 0)
    {
        delete_char(cursor, position);
        return -1;
    }

    /* A new edit invalidates the previous Redo history. */
    redo_stack->top = -1;

    return 0;
}

int delete_char_with_undo(Node *cursor, int line_number,
                          int position, Stack *undo_stack,
                          Stack *redo_stack)
{
    char character;

    if (cursor == NULL || cursor->line == NULL ||
        undo_stack == NULL || redo_stack == NULL)
        return -1;

    if (position < 0 ||
        (size_t)position >= strlen(cursor->line))
        return -1;

    character = cursor->line[position];

    if (delete_char(cursor, position) != 0)
        return -1;

    if (record_operation(undo_stack, DELETE_CHAR,
                         line_number, position, character) != 0)
    {
        insert_char(cursor, position, character);
        return -1;
    }

    /* A new edit invalidates the previous Redo history. */
    redo_stack->top = -1;

    return 0;
}
