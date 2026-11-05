#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

typedef struct Node Node;

typedef struct
{
    Node *head;
    Node *tail;
    Node *cursor;
    int cursorLine;
    int cursorPos;
} TextEditor;

void printMenu(void);
void handleCommand(TextEditor *editor, int choice);
void processCommand(TextEditor *editor, char *input);

#endif