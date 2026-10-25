#include "file_ops.h"

int validate_file_name(File* file)
{
    /* Ask for filename to be loaded */
    char file_name[100];
    printf("Enter the filename to be opened/saved : ");
    scanf("%s",file_name);
    getchar();

    /* Validate if file is .txt or not */
    char* s = strchr(file_name,'.');
    if(s == NULL)
    {
        printf("Error : Please give only .txt files with proper extension\n");
        return FAILURE;
    }

    if(strcmp(s,".txt") != 0)
    {
        printf("Error : Please give only .txt files\n");
        return FAILURE;
    }

    strcpy(file->filename,file_name);
    return SUCCESS;
}

int open_and_load_file(TextEditor* editor,DynamicArrayStack* undostack,File* file)
{
    /* Check whether file is present or not */
    file->fp = fopen(file->filename,"r");
    if(file->fp == NULL)
    {
        printf("Error : File doesnt exist\n");
        return FAILURE;
    }

    /* Set logAction to 1 to prevent undoStack from logging */
    extern int logAction;
    logAction = 1;

    /* USe buffer to read from file and load into text editor */
    char buffer[MAX_CHARS];

    /* Read line by line */
    while(fgets(buffer,MAX_CHARS,file->fp) != NULL)
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        /* Create new nodes and store each line */
        if(insertTextnewline(editor,undostack,buffer) == FAILURE)
        {
            logAction = 0;
            fclose(file->fp);
            return FAILURE;
        }
    }
    /* Set logAction to 0 for future undo logging */
    logAction = 0;
    fclose(file->fp);
    return SUCCESS;
}

int save_file(TextEditor editor,File* file)
{
    /* Check whether file is present or not */
    file->fp = fopen(file->filename,"w");
    if(file->fp == NULL)
    {
        printf("Error : File doesnt exist\n");
        return FAILURE;
    }

    /* Write to file line by line */
    while(editor.head)
    {
        fprintf(file->fp,"%s\n",editor.head->line);
        editor.head = editor.head->next;
    }

    fclose(file->fp);
    return SUCCESS;
}