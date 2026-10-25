#include "text_editor.h"
#include "cursor_navigations.h"

void printCursorMenu()
{
    printf("\n1.move up\n");
    printf("2.move down\n");
    printf("3.move left\n");
    printf("4.move right\n");
    printf("5.start of line\n");
    printf("6.end of line\n");
    printf("7.start of text editor\n");
    printf("8.end of text editor\n");
    printf("9.Exit cursor navigation\n");
}

void move_cursor_up(TextEditor* editor)
{
    if(editor->current == editor->head)
    {
        printf("Error : There is no lines to move up in text editor\n");
        return;
    }

    editor->current = editor->current->prev;
    editor->cursorLine--;

    if(editor->cursorPos > strlen(editor->current->line))
    {
        editor->cursorPos = strlen(editor->current->line);
    }
}

void move_cursor_down(TextEditor* editor)
{
    if(editor->current == editor->tail)
    {
        printf("Error : This is the last line cannot move further down\n");
        return;
    }

    editor->current = editor->current->next;
    editor->cursorLine++;

    if(editor->cursorPos > strlen(editor->current->line))
    {
        editor->cursorPos = strlen(editor->current->line);
    }
}

void move_cursor_left(TextEditor* editor)
{
    if(editor->cursorPos == 0)
    {
        if(editor->current != editor->head)
        {
            move_cursor_up(editor);
            editor->cursorPos = strlen(editor->current->line);
            return;
        }
        else
        {
            printf("Error : This is the first position cannot move further left\n");
            return;
        }
    }

    editor->cursorPos--;
}

void move_cursor_right(TextEditor* editor)
{
    if(editor->cursorPos == strlen(editor->current->line))
    {
        if(editor->current != editor->tail)
        {
            move_cursor_down(editor);
            editor->cursorPos = 0;
            return;
        }
        else
        {
            printf("Error : This is the last position cannot move right further\n");
            return;
        }
    }

    editor->cursorPos++;
}

void start_of_line(TextEditor* editor)
{
    if(editor->current == NULL)
    {
        printf("Error : Text editor is empty\n");
        return;
    }

    editor->cursorPos = 0;
}

void end_of_line(TextEditor* editor)
{
    if(editor->current == NULL)
    {
        printf("Error : Text editor is empty\n");
        return;
    }

    editor->cursorPos = strlen(editor->current->line);
}

void start_of_text_editor(TextEditor* editor)
{
    if(editor->current == NULL)
    {
        printf("Error : Text editor is empty\n");
        return;
    }

    editor->current = editor->head;
    editor->cursorLine = 1;
    editor->cursorPos = 0;
}

void end_of_text_editor(TextEditor* editor)
{
    if(editor->current == NULL)
    {
        printf("Error : Text editor is empty\n");
        return;
    }

    extern int lineCount;
    editor->current = editor->tail;
    editor->cursorLine = lineCount;
    editor->cursorPos = strlen(editor->current->line);
}
