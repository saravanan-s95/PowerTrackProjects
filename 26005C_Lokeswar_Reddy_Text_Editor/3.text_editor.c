#include "header.h"

/*
Functions in this file:
    void handleCommand(TxtEdtr *editor, DynArrStk *undoStack, DynArrStk *redoStack, char *command);
    void printMenu(void);
*/
extern char *clipboard;

void handleCommand(TxtEdtr *editor, DynArrStk *undoStack, DynArrStk *redoStack, char *command)
{
    char *token = strtok(command, " \n");

    if (token == NULL)
        return;
    
    /* Insert text command, will append text at last of current line */
    if (strcmp(token, "insert") == 0)
    {
        char *text = strtok(NULL, "\n");

        if (text == NULL)
        {
            printf("Usage: insert <text>\n");
            return;
        }

        int oldLine = editor->cline;
        int oldPos = editor->cpos;

        insertText(editor, text);
        pushStack(undoStack, "insert", text, oldLine, oldPos);
        clearStack(redoStack);
    }

    /* Insert text at new line command */
    else if (strcmp(token, "insert_new_line") == 0)
    {
        char *text = strtok(NULL, "\n");

        if (text == NULL)
        {
            printf("Usage: insert <text>\n");
            return;
        }

        appendLine(editor, text);
        pushStack(undoStack, "insert_new_line", text, editor->cline, 0);
        clearStack(redoStack);
    }


    /* delete characters command */
    else if (strcmp(token, "delete") == 0)
    {
        char *number = strtok(NULL, " \n");
        if (number == NULL)
        {
            printf("Usage: delete <number>\n");
            return;
        }

        int count = atoi(number);
        /* Validation for negative or invalid input */
        if (count <= 0)
        {
            printf("Error: Invalid character count.\n");
            return;
        }

        /* If Editor is empty or if cursor is pointing to invalid node, then we do not attempt deleting */
        if (editor == NULL || editor->cursor == NULL)
        {
            printf("Error: Editor is empty.\n");
            return;
        }

        /* Validating whether user requested no.of characters available at current line */        
        if (count <= editor->cpos)
        {
            char *deleted = malloc((size_t)count + 1);
            if (deleted == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                return;
            }

            int pos = editor->cpos - count;  //position to start deleting

            memcpy(deleted,editor->cursor->txt + pos,(size_t)count);
            deleted[count] = '\0';

            deleteCharacters(editor, count);
           
            pushStack(undoStack, "delete", deleted, editor->cline, pos);
            clearStack(redoStack);
            free(deleted);
        }
        else
        {
            printf("Error: Deletion is impossible (Requested: %d characters, available: %d characters before cursor).\n",count, editor->cpos);
        }
    }

    /* delete current line command */ 
    else if (strcmp(token, "delete_line") == 0)
    {
        
        /* If Editor is empty or if cursor is pointing to invalid node, then we do not attempt deleting */
        if (editor == NULL || editor->cursor == NULL)
        {
            printf("Error: Editor is empty.\n");
            return;
        }

        /* Validating whether current line consists any characters or not */        
        if (strlen(editor->cursor->txt) > 0)
        {
            printf("pointing: %s\n",editor->cursor->txt);
            int line_length = strlen(editor->cursor->txt);

            char *deleted = malloc((size_t)line_length + 1);
            if (deleted == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                return;
            }

            memcpy(deleted,editor->cursor->txt, (size_t)line_length);
            deleted[line_length] = '\0';

            
            pushStack(undoStack, "delete_line", deleted, editor->cline, 0);
            printf("%s\n",deleted);
            printf("%d\n",editor->cline);

            deleteLine(editor);
            clearStack(redoStack);
            free(deleted);
        }
        else
        {
            int line = editor->cline;
            deleteLine(editor);
            pushStack(undoStack, "delete_line", NULL, line, 0);
            // printf("to undo: NULL\n");
            clearStack(redoStack);
        }
    }

    /* Cursor Movement Commands */
    else if (!strcmp(token, "up") )         { moveCursorUp(editor);       }
    else if (!strcmp(token, "down") )       { moveCursorDown(editor);     }
    else if (!strcmp(token, "left") )       { moveCursorLeft(editor);     }
    else if (!strcmp(token, "right") )      { moveCursorRight(editor);    }
    else if (!strcmp(token, "start") )      { jumpToStartOfFile(editor);  }
    else if (!strcmp(token, "end") )        { jumpToEndOfFile(editor);    }
    else if (!strcmp(token, "line_start") ) { jumpToStartOfLine(editor);  }
    else if (!strcmp(token, "line_end") )   { jumpToEndOfLine(editor);    }


    /* File Opening Commands */
    else if (strcmp(token, "open") == 0)
    {
        char *filename = strtok(NULL, " \n");
        if (filename != NULL && openFile(editor, filename))
        {
            clearStack(undoStack);
            clearStack(redoStack);
        }
    }
    else if (strcmp(token, "save") == 0)
    {
        char *filename = strtok(NULL, " \n");

        if (filename != NULL)
            saveFile(editor, filename);
        else
            printf("Usage: save <file_name.txt>\n");
        
    }
    else  if (strcmp(token, "close") == 0)
    {
        closeFile(editor);
        clearStack(undoStack);
        clearStack(redoStack);
    }


    /* Find and Replace commands */
    else if (strcmp(token, "find") == 0)
    {
        char *text = strtok(NULL, "\n");
        
        if (text != NULL)
            findText(editor, text);
        else
            printf("Usage: find <word>\n");
    }
    else if (strcmp(token, "replace") == 0) 
    {
        char *oldText = strtok(NULL, " \n");
        char *newText = strtok(NULL, " \n");

        if (oldText != NULL && newText != NULL)
            replaceText(editor, oldText, newText);
        
        else
            printf("Usage: replace <old> <new>\n");
    }


    else if (strcmp(token, "cut") == 0)      
    {    
        pushStack(undoStack, "cut", editor->cursor->txt, editor->cline, editor->cpos);
        cutText(editor);
        clearStack(redoStack);  
    }
    else if (strcmp(token, "paste") == 0)    
    { 
        pushStack(undoStack, "paste", clipboard, editor->cline, editor->cpos);
        pasteText(editor); 
        clearStack(redoStack);  
    }
    else if (strcmp(token, "copy") == 0)     { copyText(editor);  }
    
    else if (strcmp(token, "undo") == 0)     { undo(editor, undoStack, redoStack);  }
    else if (strcmp(token, "redo") == 0)     { redo(editor, undoStack, redoStack);  }
    
    else if (strcmp(token, "print") == 0)    { displayText(editor);        } //rint
    else if (strcmp(token, "position") == 0) { showCursorPosition(editor); }
    else if (strcmp(token, "help") == 0)     { printMenu();                }

    else printf("Error: Unknown command '%s'.\n", token);

    return;
}


void printMenu(void)
{
    printf("\n============================================\n");
    printf("          TEXT EDITOR COMMANDS\n");
    printf("============================================\n");
    printf("insert <text>\n");
    printf("insert_new_line <text>\n");
    printf("delete <number>\n");
    printf("delete_line\n");
    printf("up\n");
    printf("down\n");
    printf("left\n");
    printf("right\n");
    printf("start\n");
    printf("end\n");
    printf("line_start\n");
    printf("line_end\n");
    printf("open <filename>\n");
    printf("save <filename>\n");
    printf("close\n");
    printf("find <text>\n");
    printf("replace <old> <new>\n");
    printf("copy\n");
    printf("cut\n");
    printf("paste\n");
    printf("undo\n");
    printf("redo\n");
    printf("print\n");
    printf("position\n");
    printf("help\n");
    printf("exit\n");
    printf("============================================\n");
}