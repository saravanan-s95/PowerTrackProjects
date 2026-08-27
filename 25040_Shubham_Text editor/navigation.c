#include "file_ops.h"
#include "redo_undo.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "display_search.h"

void navigation(TextEditor *editor)
{
    char str[20];
    printf("\n========================================\n");
    printf("         Cursor Navigation Menu\n");
    printf("========================================\n");
    printf("1. Move Cursor Up             : <up>\n");
    printf("2. Move Cursor Down           : <down>\n");
    printf("3. Move Cursor Left           : <left>\n");
    printf("4. Move Cursor Right          : <right>\n");
    printf("5. Jump to Start of File      : <start>\n");
    printf("6. Jump to End of File        : <end>\n");
    printf("7. Jump to Start of Line      : <startLine>\n");
    printf("8. Jump to End of Line        : <endLine>\n");
    printf("========================================\n");
    printf("Enter command: ");
    scanf(" %[^\n]",str);
    if((strcmp(str,"up"))==0)
    {
        
        moveCursorUp(editor);
    }
    else if((strcmp(str,"down"))==0)
    {
        moveCursorDown(editor);
    }
    else if((strcmp(str,"left"))==0)
    {
        moveCursorLeft(editor);
    }
    else if((strcmp(str,"right"))==0)
    {
        moveCursorRight(editor);
    }
    else if((strcmp(str,"start"))==0)
    {
        jumpToStartOfFile(editor);
    }
    else if((strcmp(str,"end"))==0)
    {
        jumpToEndOfFile(editor);
    }
    else if((strcmp(str,"startLine"))==0)
    {
        jumpToStartOfLine(editor);
    }
    else if((strcmp(str,"endLine"))==0)
    {
        jumpToEndOfLine(editor);
    }

}
void moveCursorUp(TextEditor *editor)
{
    if(editor->cursor==NULL || editor->cursor->prev == NULL)
    {
        
        return;
    }
    editor->cursor = editor->cursor->prev;
    if(editor->cursor->line[0]=='\0')
    {
        editor->cursorPos = 0;
    }
    if(strlen(editor->cursor->line) < editor->cursorPos)
    {
        editor->cursorPos = strlen(editor->cursor->line);
    }
    editor->cursorLine--;
}
void moveCursorDown(TextEditor *editor)
{
    if(editor->cursor==NULL || editor->cursor->next == NULL)
    {
    
        return;
    }
    editor->cursor = editor->cursor->next;
    if(strlen(editor->cursor->line) < editor->cursorPos)
    {
        editor->cursorPos=strlen(editor->cursor->line);
    }
    editor->cursorLine++;
}
void moveCursorLeft(TextEditor *editor)
{
    if(editor->cursorPos == 0)
    {
        return;
    }
    editor->cursorPos--;
}
void moveCursorRight(TextEditor *editor)
{
    editor->cursorPos++;
}
void jumpToStartOfFile(TextEditor *editor)
{
    editor->cursor = editor->head;
    editor->cursorLine=1;
    editor->cursorPos=0;
}
void jumpToEndOfFile(TextEditor *editor)
{
    
    int count=0;
    Node*temp = editor->head;
    while(temp->next!=NULL)
    {
        count++;
        temp = temp->next;
    }
    editor->cursor = temp;
    editor->cursorLine = count+1;
    editor->cursorPos = 0;
}
void jumpToStartOfLine(TextEditor *editor)
{
    editor->cursorPos = 0;
}
void jumpToEndOfLine(TextEditor *editor)
{
    if(editor->cursor==NULL)
    {
        return;
    }
    int len = strlen(editor->cursor->line);
    editor->cursorPos = len;
}