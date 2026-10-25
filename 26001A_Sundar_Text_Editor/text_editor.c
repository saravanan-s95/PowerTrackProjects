#include "text_editor.h"
#include "undo_redo.h"

/* Total no of lines present in text editor */
int lineCount = 0;

/* Flag variable for undo and redo operations */
/* 0 - Undo and 1 - Redo */
int logAction = 0;

/* clipboard fo copy cut and paste operations */
static char clipboard[MAX_CHARS];

/* Print text editor menu */
void printMenu()
{
    printf("1.insert\n");
    printf("2.delete\n");
    printf("3.move cursor\n");
    printf("4.copy\n");
    printf("5.cut\n");
    printf("6.paste\n");
    printf("7.undo\n");
    printf("8.redo\n");
    printf("9.print\n");
    printf("10.open file\n");
    printf("11.save file\n");
    printf("12.exit\n\n");
}

/* Return commands from user input */
CommandType handleCommands(char* command)
{
    if(strncmp(command,"insert",6) == 0)
    {
        /* Insert command at new line or current line */
        printf("\n1.Insert at current cursor position\n");
        printf("2.Insert at newline\n");

        printf("Enter your choice : ");
        int ins_choice;
        scanf("%d",&ins_choice);
        getchar();

        if(ins_choice == 1)
        {
            /* Insert current line command */
            return INSERT_CURRENT_LINE;
        }
        else if(ins_choice == 2)
        {
            /* Insert new line command */
            return INSERT_NEWLINE;
        }
    }

    else if(strcmp(command,"delete") == 0)
    {
        /* Delete current line no of chars or delete current line entirely */
        printf("\n1.Delete no of characters\n");
        printf("2.Delete line\n");

        printf("Enter your choice : ");
        int del_choice;
        scanf("%d",&del_choice);
        getchar();

        if(del_choice == 1)
        {
            /* Delete current line no of chars */
            return DELETE_CHARACTERS_CURRENT_POS;
        }
        else if(del_choice == 2)
        {
            /* Delete current line entirely */
            return DELETE_LINE;
        }
    }

    else if(strcmp(command,"move cursor") == 0)
    {
        /* Cursor navigation command */
        return CURSOR_NAVIGATION;
    }

    else if(strcmp(command,"copy") == 0)
    {
        /* Copy command */
        return COPY;
    }
    
    else if(strcmp(command,"cut") == 0)
    {
        /* Cut command */
        return CUT;
    }

    else if(strcmp(command,"paste") == 0)
    {
        /* Paste command */
        return PASTE;
    }

    else if(strcmp(command,"print") == 0)
    {
        /* Print text editor command */
        return PRINT;
    }

    else if(strcmp(command,"undo") == 0)
    {
        /* Undo command */
        return UNDO;
    }

    else if(strcmp(command,"redo") == 0)
    {
        /* Redo command */
        return REDO;
    }

    else if(strcmp(command,"open file") == 0)
    {
        /* Open file */
        return OPEN_FILE;
    }

    else if(strcmp(command,"save file") == 0)
    {
        /* Save file */
        return SAVE_FILE;
    }

    else if(strcmp(command,"exit") == 0)
    {
        /* Exit command */
        return EXIT;       
    }
    return UNKNOWN;
}

int insertTextnewline(TextEditor* editor,DynamicArrayStack* undoStack,char* str)
{
    if(strlen(str) >= MAX_CHARS)
    {
        
        printf("Error : Cannot add %ld characters.Only %d characters available\n",strlen(str),MAX_CHARS-editor->cursorPos);
        return FAILURE;
    }

    if(logAction == 0)
    {
        /* Increment undo stack */
        ++undoStack->size;
        if(undoStack->size >= undoStack->capacity)
        {
            --undoStack->size;
            printf("Error : Undo capacity has reached maximum\n");
            return FAILURE;
        }
    }

    /* Create new line from current line */
    Node* new = malloc(sizeof(Node));
    if(new == NULL)
    {
        printf("Error : Memory allocation failed\n");
        return FAILURE;
    }

    /* If current line is NULL */
    if(editor->current == NULL)
    {
        /* If text editor is empty */
        if(editor->head == NULL)
        {
            /* Update head and tail with new */
            editor->head = new;
            editor->tail = new;
            new->prev = NULL;
            new->next = NULL;
        }
        else
        {
            /* Insert at the beginning. This is needed when undoing a deletion of line 1. */
            new->prev = NULL;
            new->next = editor->head;
            editor->head->prev = new;
            editor->head = new;
        }
    }
    else
    {
        /* Update previous and next nodes with new */
        new->prev = editor->current;
        new->next = editor->current->next;

        /* If next line exists update its previous node with new */
        if(editor->current->next)
        {
            editor->current->next->prev = new;
        }
        else
        {
            /* Update tail with new if current line is last line */
            editor->tail = new;
        }
        editor->current->next = new;
    }

    /* Set cuurent line to new */
    editor->current = new;

    /* Copy the contents from stdin to node line */
    strcpy(editor->current->line,str);

    /* Set cursor position to the end of the text */
    editor->cursorPos = strlen(str);

    /* Increment lineCount and cursorLine */
    lineCount++;
    editor->cursorLine++;

    if(logAction == 0)
    {
        /* Store in undo stack */
        undoStack->actions[undoStack->size].operation = "insert_line";
        undoStack->actions[undoStack->size].text = malloc(strlen(str) + 1);
        strcpy(undoStack->actions[undoStack->size].text,str);
        undoStack->actions[undoStack->size].cursorLine = editor->cursorLine;
        undoStack->actions[undoStack->size].cursorPos = editor->cursorPos;
    } 
    return SUCCESS;
}

int insertText(TextEditor* editor, DynamicArrayStack* undoStack, char* str)
{
    /* If current is NULL */
    if(editor->current == NULL)
    {
        printf("Error : There is no data present in text editor\n");
        return FAILURE;
    }

    /* Find current line chars length and entered text length */
    int text_len = strlen(str);
    int len = strlen(editor->current->line);

    /* If entered text is greater than max available */
    if(text_len + len >= MAX_CHARS)
    {
        printf("Error : Cannot add %d characters.Only %d characters available\n",text_len, MAX_CHARS - len);
        return FAILURE;
    }

    if(logAction == 0)
    {
        /* Increment undo stack */
        ++undoStack->size;
        if(undoStack->size >= undoStack->capacity)
        {
            --undoStack->size;
            printf("Error : Undo capacity has reached maximum\n");
            return FAILURE;
        }
    }

    int i;

    /* Move chars to right */
    for(i = len; i >= editor->cursorPos; i--)
    {
        editor->current->line[i + text_len] = editor->current->line[i];
    }

    /* Insert the entered text in the line */
    for(i = 0;i < text_len; i++)
    {
        editor->current->line[editor->cursorPos++] = str[i];
    }

    if(logAction == 0)
    {
        /* Store in undo stack */
        undoStack->actions[undoStack->size].operation = "insert_chars";
        undoStack->actions[undoStack->size].text = malloc(text_len + 1);
        strcpy(undoStack->actions[undoStack->size].text, str);
        undoStack->actions[undoStack->size].cursorLine = editor->cursorLine;
        undoStack->actions[undoStack->size].cursorPos = editor->cursorPos;
    }

    return SUCCESS;
}

int deleteCharacters(TextEditor* editor,DynamicArrayStack* undoStack,int del_chars)
{
    /* If current is NULL */
    if(editor->current == NULL)
    {
        printf("Error : There is no data present in text editor\n");
        return FAILURE;
    }

    /* Delete left or right from cursor position */
    printf("1.Delete left from current cursor position\n");
    printf("2.Delete right from current cursor position\n");
    printf("Enter your choice : ");

    int side_choice;
    scanf("%d",&side_choice);
    getchar();

    switch(side_choice)
    {
        case 1:
        /* Delete left */
        if(deleteLeft(editor,undoStack,del_chars) == FAILURE)
        {
            return FAILURE;
        }
        break;

        case 2:
        /* Delete right */
        if(deleteRight(editor,undoStack,del_chars) == FAILURE)
        {
            return FAILURE;
        }
        break;

        default:
        /* Invalid option */
        printf("Error : Invalid option\n");
        return FAILURE;
    }
    return SUCCESS;
}

int deleteRight(TextEditor* editor, DynamicArrayStack* undoStack, int del_chars)
{
    /* If entered chars is less than 0 or greater than max available in current line */
    if(del_chars <= 0 || del_chars > strlen(editor->current->line) - editor->cursorPos)
    {
        printf("Error : Cannot delete %d characters.Only %ld characters available in right direction\n",del_chars, strlen(editor->current->line) - editor->cursorPos);
        return FAILURE;
    }

    if(logAction == 0)
    {
        /* Increment undo stack */
        ++undoStack->size;
        if(undoStack->size >= undoStack->capacity)
        {
            --undoStack->size;
            printf("Error : Undo capacity has reached maximum\n");
            return FAILURE;
        }

        /* Allocate memory for text to be stored in undo stack */
        undoStack->actions[undoStack->size].text = malloc(del_chars + 1);

        int start = editor->cursorPos;

        /* Copy the remaining text in undostack */
        for(int i = 0; i < del_chars; i++)
        {
            undoStack->actions[undoStack->size].text[i] = editor->current->line[start++];
        }
        undoStack->actions[undoStack->size].text[del_chars] = '\0';
    }

    /* Delete the characters and left shift any remaining characters*/
    int i;
    for(i = editor->cursorPos + del_chars;editor->current->line[i] != '\0';i++)
    {
        editor->current->line[i - del_chars] = editor->current->line[i];
    }
    editor->current->line[i - del_chars] = '\0';

    if(logAction == 0)
    {
        /* Store in undo stack */
        undoStack->actions[undoStack->size].operation = "delete_chars";
        undoStack->actions[undoStack->size].cursorLine = editor->cursorLine;
        undoStack->actions[undoStack->size].cursorPos = editor->cursorPos;
    }
    return SUCCESS;
}

int deleteLeft(TextEditor* editor, DynamicArrayStack* undoStack, int del_chars)
{
    /* If entered chars is less than 0 or greater than max available */
    if(del_chars <= 0 || del_chars > editor->cursorPos)
    {
        printf("Error : Cannot delete %d characters.Only %d characters available in left direction\n",del_chars, editor->cursorPos);
        return FAILURE;
    }

    if(logAction == 0)
    {
        /* Increment undo stack */
        ++undoStack->size;
        if(undoStack->size >= undoStack->capacity)
        {
            --undoStack->size;
            printf("Error : Undo capacity has reached maximum\n");
            return FAILURE;
        }

        int start = editor->cursorPos - del_chars;

        /* Alloate memory for undo stack text */
        undoStack->actions[undoStack->size].text = malloc(del_chars + 1);

        /* Store the rest of chars in undo stack text */
        for(int i = 0; i < del_chars; i++)
        {
            undoStack->actions[undoStack->size].text[i] = editor->current->line[start++];
        }

        undoStack->actions[undoStack->size].text[del_chars] = '\0';
    }

    /* Delete the actual characters and shift left*/
    int start = editor->cursorPos - del_chars;
    int i;

    for(i = editor->cursorPos;editor->current->line[i] != '\0';i++)
    {
        editor->current->line[start++] = editor->current->line[i];
    }

    editor->current->line[start] = '\0';
    editor->cursorPos -= del_chars;

    if(logAction == 0)
    {
        /* Store in undo stack */
        undoStack->actions[undoStack->size].operation = "delete_chars";
        undoStack->actions[undoStack->size].cursorLine = editor->cursorLine;
        undoStack->actions[undoStack->size].cursorPos = editor->cursorPos;
    }
    return SUCCESS;
}

int deleteLine(TextEditor* editor,DynamicArrayStack* undoStack)
{
    /* If current line is NULL */
    if(editor->current == NULL)
    {
        printf("Error : There is no data present in text editor\n");
        return FAILURE;
    }

    /* To store in undo stack if first line is deleted */
    int deletedLine = editor->cursorLine;

    if(logAction == 0)
    {
        /* Increment stack */
        ++undoStack->size;
        if(undoStack->size >= undoStack->capacity)
        {
            --undoStack->size;
            printf("Error : Undo capacity has reached maximum\n");
            return FAILURE;
        }

        /* Store in stack */
        undoStack->actions[undoStack->size].operation = "delete_line";
        undoStack->actions[undoStack->size].text = malloc(strlen(editor->current->line) + 1);
        strcpy(undoStack->actions[undoStack->size].text,editor->current->line);
        undoStack->actions[undoStack->size].cursorLine = deletedLine;
        undoStack->actions[undoStack->size].cursorPos = editor->cursorPos;
    }

    Node* temp = editor->current;

    /* If next line exists */
    if(temp->next)
    {
        /* Update next line to previous line nodes */
        editor->current = temp->next;
        temp->next->prev = temp->prev;
        editor->cursorLine = deletedLine;
    }
    else if(temp->prev)
    {
        /* Move current to previous line and update previous node and tail */
        editor->current = temp->prev;
        temp->prev->next = NULL;
        editor->tail = temp->prev;
        editor->cursorLine = deletedLine - 1;
    }
    else
    {
        /* If only one line is present and was deleted */
        editor->head = NULL;
        editor->tail = NULL;
        editor->current = NULL;
        editor->cursorLine = 0;
    }

    if(temp->prev == NULL)
    {
        /* If deleted line was first line */
        editor->head = editor->current;
        if(editor->current)
        {
            editor->current->prev = NULL;
        }
    }
    else
    {
        temp->prev->next = temp->next;
    }

    /* Free the node */
    free(temp);

    /* Decrement the line count */
    lineCount--;

    /* Set cursor position based on line exists or not */
    if(editor->current)
    {
        editor->cursorPos = strlen(editor->current->line);
    }
    else
    {
        editor->cursorPos = 0;
    }
    return SUCCESS;
}

/* Print the text editor line by line */
void printTextEditor(TextEditor editor)
{
    printf("\n\n------------------------------------------------------------------------\n");

    /* Print current cursor position */
    int i = 1;
    while(editor.head)
    {
        if(editor.head == editor.current)
        {
            printf("Line %d: ",i++);
            int j = 0;
            while(editor.head->line[j])
            {
                printf("%c",editor.head->line[j++]);
                if(j == editor.cursorPos)
                {
                    printf("|");
                }
            }
            printf("\n");
        }
        else
        {
            printf("Line %d: %s\n",i++,editor.head->line);
        }
        editor.head = editor.head->next;
    }

    printf("Cursor position: Line %d, Column %d\n",editor.cursorLine,editor.cursorPos);
    printf("------------------------------------------------------------------------\n");
}

/* Copy Text function */
int copyText(TextEditor editor)
{
    /* If text editor is empty */
    if(editor.current == NULL)
    {
        printf("Error : There is no data present in text editor\n");
        return FAILURE;
    }

    /* Copy specific chars in current line or copy entire current line */
    printf("1.Copy no of charcters\n");
    printf("2.Copy entire line\n");

    int copy_choice;
    printf("Enter your choice : ");
    scanf("%d",&copy_choice);
    getchar();

    switch(copy_choice)
    {
        case 1:
        {
            /* Copy chars in current line */
            /* Enter no of chars to be copied in current line */
            int copy_chars;
            printf("Enter the no of characters to be copied : ");
            scanf("%d",&copy_chars);
            getchar();

            /* Copy to the left or right of cursor position */
            printf("1.Copy left from current cursor position\n");
            printf("2.Copy right from current cursor position\n");

            int copy_side_choice;
            printf("Enter your choice : ");
            scanf("%d",&copy_side_choice);
            getchar();

            if(copy_side_choice == 1)
            {
                /* Copy left */

                if(copyLeft(editor,copy_chars) == FAILURE)
                {
                    printf("Error : Cannot copy %d characters.Only %d characters available in left direction\n",copy_chars,editor.cursorPos);
                    return FAILURE;
                }
            }
            else if(copy_side_choice == 2)
            {
                /* Copy right */

                if(copyRight(editor,copy_chars) == FAILURE)
                {
                    printf("Error : Cannot copy %d characters.Only %ld characters available in right direction\n",copy_chars,strlen(editor.current->line) - editor.cursorPos);
                    return FAILURE;
                }
            }
            else
            {
                /* Invalid */
                printf("Invalid option\n");
                return FAILURE;
            }
        }

        break;

        case 2:

        /* Copy entire current line and store in clipboard */
        strcpy(clipboard,editor.current->line);
        break;

        /* Invalid */
        default:
        printf("Invaild option\n");
    }
    return SUCCESS;
}

int copyLeft(TextEditor editor,int copy_chars)
{
    /* If required chars is less than 0 or greater than the chars available to the left of current cursor position */
    if(copy_chars < 0 || copy_chars > editor.cursorPos)
    {
        return FAILURE;
    }

    /* Move cursor position to the start of the char to be copied from left*/
    editor.cursorPos = editor.cursorPos - copy_chars;
    int i;

    /* Copy till required no of chars is reached and store in clipboard */
    for(i=0;i<copy_chars;i++)
    {
        clipboard[i] = editor.current->line[editor.cursorPos++];
    }
    clipboard[i] = '\0';
    return SUCCESS;
}

int copyRight(TextEditor editor,int copy_chars)
{
    /* If no of chars is negative or more than max chars available in text editor */
    if(copy_chars < 0 || copy_chars > strlen(editor.current->line) - editor.cursorPos)
    {
        return FAILURE;
    }

    int i=0;
    
    /* Copy till required chars is reached and store in clipboard */
    for(i=0;i<copy_chars;i++)
    {
        clipboard[i] = editor.current->line[editor.cursorPos++];
    }
    clipboard[i] = '\0';
    return SUCCESS;
}

int cutText(TextEditor* editor,DynamicArrayStack* undoStack)
{
    /* If text editor is empty */
    if(editor->current == NULL)
    {
        printf("Error : There is no data present in text editor\n");
        return FAILURE;
    }

    /* Cut no of chars or entire line */
    printf("1.Cut no of charcters\n");
    printf("2.Cut entire line\n");

    int cut_choice;
    printf("Enter your choice : ");
    scanf("%d",&cut_choice);
    getchar();

    switch(cut_choice)
    {
        case 1:
        {
            /* Cut specific no of chars */
            /* Enter specific no of chars to be cut */
            int cut_chars;
            printf("Enter the no of characters to be cut : ");
            scanf("%d",&cut_chars);
            getchar();

            /* Cut left or right from current cursor position */
            printf("1.Cut left from current cursor position\n");
            printf("2.Cut right from current cursor position\n");

            int cut_side_choice;
            printf("Enter your choice : ");
            scanf("%d",&cut_side_choice);
            getchar();

            if(cut_side_choice == 1)
            {
                /* Cut left from current cursor position */

                /* First copy left the required chars and store in clipboard */
                if(copyLeft(*editor,cut_chars) == FAILURE)
                {   
                    printf("Error : Cannot copy %d characters.Only %d characters available in left direction\n",cut_chars,editor->cursorPos);
                    return FAILURE;
                }
                /* Then delete left the chars */
                deleteLeft(editor,undoStack,cut_chars);
            }
            else if(cut_side_choice == 2)
            {
                /* Cut right from current cursor position */

                /* First copy right the required chars and store in clipboard */
                if(copyRight(*editor,cut_chars) == FAILURE)
                {
                    printf("Error : Cannot copy %d characters.Only %ld characters available in right direction\n",cut_chars,strlen(editor->current->line) - editor->cursorPos);
                    return FAILURE;
                }
                /* Then delete right the chars */
                deleteRight(editor,undoStack,cut_chars);
            }
            else
            {
                /* Invalid */
                printf("Error : Invalid option\n");
                return FAILURE;
            }
        }

        break;

        case 2:
        /* Cut entire current line */

        /* Copy current line and store in clipboard */
        strcpy(clipboard,editor->current->line);
        /* Delete current line */
        deleteLine(editor,undoStack);
        break;

        default:
        /* Invalid */
        printf("Error : Invalid option\n");
    }
    return SUCCESS;
}

int pasteText(TextEditor* editor,DynamicArrayStack* undoStack)
{
    /* Paste at current cursor positon or newline */

    printf("1.Paste at current cursor position\n");
    printf("2.Paste in newline\n");

    int paste_choice;
    printf("Enter your choice : ");
    scanf("%d",&paste_choice);
    getchar();

    switch(paste_choice)
    {
        case 1:
        /* Paste clipboard contents at current cursor position by calling insertText function */

        if(insertText(editor,undoStack,clipboard) == FAILURE)
        {
            return FAILURE;
        }
        break;

        case 2:
        /* Paste clipboard contents at newline by calling insertTextnewline function */

        if(insertTextnewline(editor,undoStack,clipboard) == FAILURE)
        {
            return FAILURE;
        }
        break;

        default:
        /* Invalid */
        printf("Error : Invalid option\n");
    }
    return SUCCESS;
}