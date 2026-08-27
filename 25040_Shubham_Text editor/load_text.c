#include "file_ops.h"
#include "redo_undo.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "display_search.h"
int load_from_file(char *argv[],TextEditor *editor)
{
    FILE *fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        perror("fopen");
        return -1;
    }
    char str[200];
    while ((fscanf(fp," %[^\n]",str))!=EOF)
    {
        Node *new = malloc(1*sizeof(Node));
        new->prev = NULL;
        new->next = NULL;
        if(editor->head == NULL && editor->tail==NULL)
        {
            editor->head = new;
            editor->tail = new;
            strcpy(new->line,str);
        }
        else
        {
            Node *temp = editor->tail;
            editor->tail->next = new;
            new->prev = editor->tail;
            editor->tail = new;
            strcpy(new->line,str);
        }
    }
    editor->cursor = editor->head;
    fclose(fp);
    

}
int save_into_file(char *arr,TextEditor *editor)
{
    FILE* fp = fopen(arr,"w");
    if(fp == NULL)
    {
        perror("fopen");
        return -1;
    }
    while(editor->head !=NULL)
    {
        fprintf(fp,"%s\n",editor->head->line);
        editor->head = editor->head->next;
    }
    fclose(fp);
}
// void showCursorPosition(TextEditor *editor)
// {
//     printf("Line")
// }