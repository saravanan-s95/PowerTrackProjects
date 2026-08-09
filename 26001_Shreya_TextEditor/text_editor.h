#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#define MAX_LEN 1000
#define CMD_LEN 30

typedef struct DynamicArrayStack DynamicArrayStack;

typedef struct lineNode{
    char buf[MAX_LEN];
    struct lineNode*prev;
    struct lineNode*next;
}line;

typedef struct editorNode{
    line*head;
    line*tail;
    line*cursor;
    int cursorLine;
    int cursorPos;
}TextEditor;

void printMenu();
void handleCommand(TextEditor*,char*,char*,DynamicArrayStack*,DynamicArrayStack*,char*);
void save_to_file(TextEditor text_Editor,char*);
int load_from_file(TextEditor*text_editor,char*filename);
void save_and_exit(TextEditor text_Editor,char*filename);
#endif