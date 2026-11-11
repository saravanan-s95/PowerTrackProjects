#include "text_editor.h"

//create node for editor
Node *createNode(const char *txt)
{
    Node *n = malloc(sizeof(Node));      //memory allocation
    n->text = strdup(txt ? txt :"");     //malloc+strcpy
    n->prev = NULL;                      //initially prev and next as NULL
    n->next = NULL;
    return n;                            
}

//create editor 
TextEditor *createEditor()
{
    TextEditor *ed = calloc(1, sizeof(TextEditor));  //all pointers become null

    //first empty line
    Node *f = createNode("");

    //initially all are pointing to first line 
    //head point to first line of file
    ed->head = f;

    //tail point to last line of file
    ed->tail = f;

    //crusor points to current line of file
    ed->cursor = f;

    //cursor starts line at 0
    ed->cursorLine = 0;
    ed->cursorPos = 0;

    //we have currently one line
    ed->totallines = 1;

    
    //initialize undo stack
    ed->undoStack.capacity = 100;
    ed->undoStack.size = 0;
    ed->undoStack.actions = malloc(sizeof(Action) *100);

    //initialize redo stack
    ed->redoStack.capacity = 100;
    ed->redoStack.size = 0;
    ed->redoStack.actions = malloc(sizeof(Action) *100);


    return ed;
}

//free editor
void freeeditor(TextEditor *ed)
{
    Node *c = ed->head;

    while(c)
    {
        //save next before freeing curent
        Node *nextNode = c->next;

        //free the line text
        free(c->text);

        //free the node
        free(c);

        //move to nextnode
        c = nextNode;
    }

    //free undo stack
    for(int i = 0; i < ed->undoStack.size;i++)
    {
        free(ed->undoStack.actions[i].operation);
        free(ed->undoStack.actions[i].text);
    }

    //free redo stack
    for(int i = 0; i < ed->redoStack.size;i++)
    {
        free(ed->redoStack.actions[i].operation);
        free(ed->redoStack.actions[i].text);
    }

    //free the dynamic array
    free(ed->undoStack.actions);
    free(ed->redoStack.actions);


    //free editor
    free(ed);
}

