#ifndef TEXT_EDITOR
#define TEXT_EDITOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdio_ext.h>
#define INITIAL_CAPACITY 10

// Define the structure for a node in the linked list (each line in the editor)
typedef struct Node {
    char line[256];
    struct Node *prev;
    struct Node *next;
} Node;

// Define the structure for the text editor
typedef struct {
    Node *head;       // Pointer to the first line
    Node *tail;       // Pointer to the last line
    Node *cursor;     // Pointer to the current line where the cursor is
    int cursorLine;   // Line number of the cursor
    int cursorPos;    // Column position of the cursor
} TextEditor;

// Define the structure for an action (used for undo/redo operations)
typedef struct {
    char operation[20];  // Operation type ("insert" or "delete")
    char text[200];       // Text involved in the operation
    int cursorLine;   // Line number where the operation occurred
    int cursorPos; 
    Node* cur;   // Column position where the operation occurred
} Action;

// Define the structure for a dynamic array stack (used for undo/redo stacks)
typedef struct {
    Action *actions;  // Dynamic array of actions
    int size;         // Current size of the stack
    int capacity;     
    // Current capacity of the stack
} DynamicArrayStack;

#endif
