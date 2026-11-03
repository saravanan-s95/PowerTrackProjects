#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include "text_editor.h"

#define INSERT_CHAR 1
#define DELETE_CHAR 2


typedef struct
{
    int operation;
    int line_number;
    int position;
    char character;
} EditOperation;

typedef struct
{
    EditOperation *array;
    int top;
    int capacity;
} Stack;


int init_stack(Stack *stack);
int push(Stack *stack, EditOperation operation);
int is_empty(Stack *stack);
int pop(Stack *stack, EditOperation *operation);
int record_operation(Stack *stack, int operation,int line_number, int position, char character);
int undo_operation(Stack *undo_stack, Stack *redo_stack,Node *head);
int redo_operation(Stack *undo_stack, Stack *redo_stack, Node *head);
int insert_char_with_undo(Node *cursor, int line_number, int position,char character, Stack *undo_stack,Stack *redo_stack);
int delete_char_with_undo(Node *cursor, int line_number, int position,Stack *undo_stack,Stack *redo_stack);


#endif