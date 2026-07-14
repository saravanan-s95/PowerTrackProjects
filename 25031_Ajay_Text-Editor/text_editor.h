#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#define MAX_STACK 100 // Max undo,redo opertaions stack can hold
#define MAX_LINE_LEN 100

// DLL to acts as a document to store the lines, each node represents one line
typedef struct node
{
    char line[256]; // Maximum 256 char can be present in one line , used to store the data
    struct node *prev;
    struct node *next;
} Node;

// It acts as memory of what just happened,so it can be reversed later
typedef struct action
{
    char operation[10]; // Stores "Insert" or "Delete"
    char text[50];      // The line of text inserted or deleted
    int line_no;        // Line number of text involved (mostly used for delete logic)
} Action;

typedef struct stack
{
    Action actions[100]; // Arrays holding upto 100 actions
    int top;             // index of topmost(Recent) action
} Stack;

// 2 instances of stack are created undo_stack and redo_stack so that we have LIFO history of what to undo and LIFO history of what was undone so that it can redone
extern Stack undo_stack;
extern Stack redo_stack;
extern Node *head;

void insert_text();
void delete_text();
void undo();
void redo();
void print_text();
void save();
void open_file();
void initialize_stack(Stack *s);
int is_empty(Stack *s);
int is_full(Stack *s);
void push(Stack *s, Action act);
Action pop(Stack *s);

#endif
