#include "header.h"


/*
Functions in this file:
    int openFile(TxtEdtr *editor, const char *fname);
    int saveFile(TxtEdtr *editor, const char *fname)
    void closeFile(TxtEdtr *editor)
*/

int openFile(TxtEdtr *editor, const char *fname)
{
    if (editor == NULL || fname == NULL)
        return 0;
    
    /* Opening User given File */
    FILE *file = fopen(fname, "r");
    if (file == NULL)
    {
        perror("Opening File Failed");
        return 0;
    }

    /* Clearing data from present editor and resetting editor with initial values*/
    closeFile(editor);

    /* Copying data from Newly opened File to New Editor */
    char buffer[MAX_LINE_LENGTH];
    char ch;
    int i=0;
    while (fread(&ch,1,1,file))
    {
        if(ch == '\r')
            fread(&ch,1,1,file);

        if(ch == '\n')
        {
            buffer[i]='\0';
            appendLine(editor, buffer);

            memset(buffer,0,i);
            i=0;
        }
        else
            buffer[i++] = ch; 
    }
    buffer[i]='\0';
    appendLine(editor, buffer);

    /* Closing File, content already copied */
    fclose(file);

    /* After opening file, cursor should point to Start of the File*/
    if (editor->head != NULL)
    {
        editor->cursor = editor->head;
        editor->cline = 1;
        editor->cpos = 0;
    }

    printf("File '%s' opened successfully.\n",fname);
    return 1;
}


int saveFile(TxtEdtr *editor, const char *fname)
{
    Node *current;

    if (editor == NULL)
        return 0;

    FILE *file = fopen(fname, "w");
    if (file == NULL)
    {
        perror("Failed in opening file\n");
        return 0;
    }

    current = editor->head;

    while (current != NULL)
    {
        fprintf(file, "%s\n", current->txt);
        current = current->nxt;
    }

    fclose(file);
    printf("File saved successfully as '%s'.\n",fname);

    return 1;
}


void closeFile(TxtEdtr *editor)
{
    if (editor == NULL)
        return;
    
    freeEditor(editor);
    initEditor(editor);

    printf("File closed successfully.\n");
}