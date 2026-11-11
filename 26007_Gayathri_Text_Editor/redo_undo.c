#include "redo_undo.h"
#include "text_editor.h"
#include "cursor_nav.h"
#include "text_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initStack(DynamicArrayStack *stack) 
{

    // Check if stack is not initialized yet 
    if (stack->actions == NULL || stack->capacity == 0) 
    {
        // Set initial capacity to 10 actions
        stack->capacity = 10;

        // Set current size to 0
        stack->size = 0;

        // Allocate memory for 10 actions
        stack->actions = malloc(sizeof(Action) * stack->capacity);
    }
}

void pushOperation(DynamicArrayStack *stack, const char *op, const char *txt, int line, int col) 
{

    // Make sure the stack is initialized before pushing
    initStack(stack);
    if (stack->size >= stack->capacity) 
    {

        // Double the capacity to make more space
        stack->capacity *= 2;

         // Reallocate memory for the bigger capacity
        stack->actions = realloc(stack->actions, sizeof(Action) * stack->capacity);
    }
    stack->actions[stack->size].operation = strdup(op ? op : "insert");

    // Copy the text involved in this operation, default is single space
    stack->actions[stack->size].text = strdup(txt ? txt : " ");

    // Save the line number where operation happened
    stack->actions[stack->size].cursorLine = line;

    // Save the column position where operation happened
    stack->actions[stack->size].cursorPos = col;
    stack->size++;
}

void clearRedo(TextEditor *ed) 
{

    // Check if editor is invalid
    if (!ed) 
        return;
    for (int i = 0; i < ed->redoStack.size; i++) 
    {

        // If operation string exists, free its memory
        if(ed->redoStack.actions[i].operation) 
            free(ed->redoStack.actions[i].operation);

        // Set operation pointer to NULL after freeing    
        ed->redoStack.actions[i].operation = NULL;

        // If text string exists, free its memory
        if(ed->redoStack.actions[i].text) 
            free(ed->redoStack.actions[i].text);

        // Set text pointer to NULL after freeing    
        ed->redoStack.actions[i].text = NULL;
    }

    // Reset redo stack size to 0 (empty)
    ed->redoStack.size = 0;
}

void performUndo(TextEditor *ed) 
{
    if (ed->undoStack.size == 0)
    {
        printf("Nothing to undo.\n");
        return;
    }

    // Get the last action from undo stack and reduce size
    Action act = ed->undoStack.actions[--ed->undoStack.size];
    
    if (strcmp(act.operation, "insert") == 0) 
    {
        Node *line = ed->cursor;

        // delete char at act.cursorPos
        int pos = act.cursorPos;
        if(line && line->text && pos < strlen(line->text))
        {

            // Shift text left to delete the character at pos
            memmove(&line->text[pos], &line->text[pos+1], strlen(line->text)-pos);
        }

        // Move cursor back to the deleted position
        ed->cursorPos = pos;

        // Save this operation in redo stack so it can be redone
        pushOperation(&ed->redoStack, "insert", act.text, act.cursorLine, act.cursorPos);
    }

    else if (strcmp(act.operation, "delete") == 0) 
    {
        // Get the current line
        Node *line = ed->cursor;

        // Get the position where text was deleted
        int pos = act.cursorPos;

        //Get current line length
        int len = line->text ? strlen(line->text) : 0;

        // Get length of deleted text
        int delLen = strlen(act.text);

        // Allocate more memory to fit the deleted text back
        char *new_text = realloc(line->text, len + delLen + 1);

        if(new_text)
        {
            // Update line text pointer
            line->text = new_text;

            // Shift text right to make space for deleted text
            memmove(&line->text[pos+1], &line->text[pos], len - pos + 1);

             // Copy the deleted text back into its original position
            memcpy(&line->text[pos], act.text, delLen);
        }

        // Move cursor after the re-inserted text
        ed->cursorPos = pos + 1;

        // Save this operation in redo stack
        pushOperation(&ed->redoStack, "delete", act.text, act.cursorLine, act.cursorPos);
    }

    
    else if (strcmp(act.operation, "newline") == 0) 
    {
        // undo newline = merge current line with next
        Node *curr = ed->cursor;
        // ed->cursor is new line after newline, so prev is old line
        if(curr && curr->prev)
        {
            Node *prev = curr->prev;

            // merge
            int prevLen = prev->text ? strlen(prev->text) : 0;
            int currLen = curr->text ? strlen(curr->text) : 0;
            char *merged = realloc(prev->text, prevLen + currLen + 1);

            if(merged)
            {
                prev->text = merged;
                strcat(prev->text, curr->text ? curr->text : "");
            }

            // unlink curr
            prev->next = curr->next;
            if(curr->next) 
                curr->next->prev = prev;
            if(ed->tail == curr) 
                ed->tail = prev;

            // Move cursor back to previous line
            ed->cursor = prev;
            ed->cursorPos = act.cursorPos;

            // Decrease cursor line number
            ed->cursorLine--;

             // Decrease total line count
            ed->totallines--;
            free(curr->text);
            free(curr);
        }

        // Save this operation in redo stack
        pushOperation(&ed->redoStack, "newline", "", act.cursorLine, act.cursorPos);
    }

    // Print undo success message
    printf("Undo: %s done\n", act.operation);

    // Free memory of the action just undone
    free(act.operation); 
    free(act.text);
    act.operation = NULL;
    act.text = NULL;
}

// Function to perform redo
void performRedo(TextEditor *ed) 
{
     // Check if redo stack is empty
    if (ed->redoStack.size == 0) 
    {
        printf("Nothing to redo.\n");
        return;
    }
    Action act = ed->redoStack.actions[--ed->redoStack.size];

    // If operation was "insert", redo means insert it again
    if (strcmp(act.operation, "insert") == 0) 
    {
        // Get the current line
        Node *line = ed->cursor;

        // Get the position where to insert
        int pos = act.cursorPos;

        // Get current line length
        int len = line->text ? strlen(line->text) : 0;

        // Allocate memory for one more character + null terminator
        char *new_text = realloc(line->text, len + 2);
        if(new_text)
        {
             // Update line text pointer
            line->text = new_text;

            // Shift text right to make space for the character
            memmove(&line->text[pos+1], &line->text[pos], len - pos + 1);

            // Insert the character again
            line->text[pos] = act.text[0];

            // Move cursor after the inserted character
            ed->cursorPos = pos + 1;
        }
    }

    else if (strcmp(act.operation, "delete") == 0) 
    {

        // Get the current line
        Node *line = ed->cursor;

        // Get the position where to delete
        int pos = act.cursorPos;

        // Get length of text that was deleted
        int delLen = strlen(act.text);
        
        if(line && line->text)

            // Shift text left to delete the text again
            memmove(&line->text[pos], &line->text[pos+delLen], strlen(line->text)-pos-delLen+1);

        // Move cursor to the delete position    
        ed->cursorPos = pos;
    }

    else if (strcmp(act.operation, "newline") == 0) 
    {
        // redo newline = split again
        insertNewLine(ed);
        
        // Remove the extra undo entry created by insertNewLine
        if(ed->undoStack.size > 0) 
            ed->undoStack.size--;
    }

    // Save this operation back into undo stack
    pushOperation(&ed->undoStack, act.operation, act.text, act.cursorLine, act.cursorPos);

    // Print redo success message
    printf("Redo: %s done\n", act.operation);

    // Free memory of the action just redone
    free(act.operation); 
    free(act.text);
    act.operation = NULL;
    act.text = NULL;
}