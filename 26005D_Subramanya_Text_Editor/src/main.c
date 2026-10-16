#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text_editor.h"
#include "cursor_navigations.h"
#include "redo_undo.h"
#include "text_ops.h"



int main(void)
{
    TextEditor editor;
    DynamicArrayStack undoStack = { NULL, 0, 0 };
    DynamicArrayStack redoStack = { NULL, 0, 0 };

    char filename[256];
    char line[256];
    char *arg;
    char *spacePos;
    size_t len;

    /* Ask for a filename upfront, since openFile needs one before anything else can run */
    printf("Text Editor v1.0\n");
    printf("Enter filename to open: ");
    fgets(filename, sizeof(filename), stdin);

    /* Strip trailing newline from filename, same trick used everywhere else */
    len = strlen(filename);
    if(len > 0 && filename[len - 1] == '\n')
        filename[len - 1] = '\0';

    /* Exit early if file doesn't open — editor never touched in that case */
    if(openFile(&editor, filename) != SUCCESS)
    {
        return 1;
    }

    /* Print the menu */
    system("clear");
    printf("\n");
    printf("%s=========================================================%s\n", COLOR_HEADER, COLOR_RESET);
    printf("%s              TEXT EDITOR v1.0 COMMANDS LISTED BY CATEGORY%s\n", COLOR_STATUS, COLOR_RESET);
    printf("%s=========================================================%s\n", COLOR_HEADER, COLOR_RESET);
    printf("%s  Editing:%s   insert <text>/ delete <n> / cut <n> / copy <n> / paste \n", COLOR_LABEL, COLOR_RESET);
    printf("%s  History:%s   undo                 redo\n", COLOR_LABEL, COLOR_RESET);
    printf("%s  Cursor: %s   goto <line> up / down / left / right / start / end\n", COLOR_LABEL, COLOR_RESET);
    printf("%s  File:   %s   save <file>          print\n", COLOR_LABEL, COLOR_RESET);
    printf("%s  Other:  %s   exit\n", COLOR_LABEL, COLOR_RESET);
    printf("%s=========================================================%s\n\n", COLOR_HEADER, COLOR_RESET);

    /* Main command loop — runs until user types exit */
    while(1)
    {
        printf("\nEnter command: ");

        if(fgets(line, sizeof(line), stdin) == NULL) break;   /* EOF safety */

        /* Strip trailing newline left behind by fgets */
        len = strlen(line);
        if(len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        /* Split into command word + argument at the first space */
        arg = NULL;
        spacePos = strchr(line, ' ');
        if(spacePos != NULL)
        {
            *spacePos = '\0';
            arg = spacePos + 1;
        }

        if(strcmp(line, "insert") == 0)
        {
            if(arg == NULL)
            {
                printf("Usage: insert <text>\n");
                continue;
            }
            if(insertText(&editor, editor.cursorLine, editor.cursorPos, arg, &undoStack) != SUCCESS)
            {
                continue;   /* error already printed inside; skip clear so message stays visible */
            }
        }
        else if(strcmp(line, "delete") == 0)
        {
            if(arg == NULL)
            {
                printf("Usage: delete <num_chars>\n");
                continue;
            }
            int numChars = atoi(arg);   /* converts "5" -> 5; invalid input silently becomes 0 */
            if(deleteCharacters(&editor, editor.cursorLine, editor.cursorPos, numChars, &undoStack) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "undo") == 0)
        {
            if(undo(&editor, &undoStack, &redoStack) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "redo") == 0)
        {
            if(redo(&editor, &undoStack, &redoStack) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "goto") == 0)
        {
            if(arg == NULL)
            {
                printf("Usage: goto <line_number>\n");
                continue;
            }
            int targetLine = atoi(arg);
            if(moveCursorToLine(&editor, targetLine) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "up") == 0)
        {
            if(moveCursorUp(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "down") == 0)
        {
            if(moveCursorDown(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "left") == 0)
        {
            if(moveCursorLeft(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "right") == 0)
        {
            if(moveCursorRight(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "start") == 0)
        {
            if(moveCursorStart(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "end") == 0)
        {
            if(moveCursorEnd(&editor) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "copy") == 0)
        {
            if(arg == NULL) { printf("Usage: copy <num_chars>\n"); continue; }
            int n = atoi(arg);
            if(copyText(&editor, editor.cursorPos, editor.cursorPos + n) != SUCCESS) { continue; }
        }
        else if(strcmp(line, "cut") == 0)
        {
            if(arg == NULL) { printf("Usage: cut <num_chars>\n"); continue; }
            int n = atoi(arg);
            if(cutText(&editor, editor.cursorPos, editor.cursorPos + n, &undoStack) != SUCCESS) { continue; }
        }
        else if(strcmp(line, "paste") == 0)
        {
            if(pasteText(&editor, &undoStack) != SUCCESS) { continue; }
        }
        else if(strcmp(line, "print") == 0)
        {
            /* No-op — auto-print at bottom of loop already shows the state */
        }
        else if(strcmp(line, "save") == 0)
        {
            if(arg == NULL)
            {
                printf("Usage: save <filename>\n");
                continue;
            }
            if(saveFile(&editor, arg) != SUCCESS)
            {
                continue;
            }
        }
        else if(strcmp(line, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("Unknown command '%s'.\n", line);
            continue;   /* unknown command message would also get wiped otherwise */
        }

        printEditor(&editor);   /* auto-show state after every command, screen cleared first */
    }

    /* Frees all nodes, resets editor to NULL state, before exiting */
    closeFile(&editor);
    printf("Goodbye!\n");

    return 0;
}