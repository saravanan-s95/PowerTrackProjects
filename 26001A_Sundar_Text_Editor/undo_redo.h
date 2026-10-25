#ifndef UNDO_REDO_H
#define UNDO_REDO_H

typedef struct TextEditor TextEditor;

#define STACK_CAPACITY 50

typedef struct
{
    char* operation; //Operation type("insert" or "delete")
    char* text; //Text involved in the operation
    int cursorLine; //Line number where operation occured
    int cursorPos; //Column position where operation occured
}Action;

typedef struct
{
    Action* actions; //Dynamic array of actions
    int size; //Current size of the stack
    int capacity; //Current capacity of the stack
}DynamicArrayStack;

/* Initialize undo and redo stacks */
int initialize_Undo_Redo_Stack(DynamicArrayStack* undoStack,DynamicArrayStack* redoStack);

/* Undo operation */
int undo_operation(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack);

/* Redo operation */
int redo_operation(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack);

/* Reset redo stack */
void reset_redo(DynamicArrayStack* redoStack);

/* Delete text editor */
void delete_text_editor(TextEditor* editor,DynamicArrayStack* undoStack,DynamicArrayStack* redoStack);

#endif