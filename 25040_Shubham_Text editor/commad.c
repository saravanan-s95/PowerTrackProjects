#include "file_ops.h"
#include "redo_undo.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "display_search.h"
extern DynamicArrayStack undoStack;
extern DynamicArrayStack redoStack;

void handleCommand(char *str,TextEditor *editor)
{
    int i=0,count=0;
    while(str[i])
    {
        if(str[i]==' ')
        {
            str[i]='\0';
            count=1;
            break;
        }
        i++;
    }
    if((strcmp(str,"insert"))==0)
    {
        if(count==1)
        {
            insertText(editor,str+(i+1));    
        }
        else 
            insertText(editor,NULL);
    }
    else if((strcmp(str,"print"))==0)
    {
        displayText(editor);
    }
    else if((strcmp(str,"delete"))==0)
    {
        int i;
        printf("What you want to delete characters or line \n1.character\n2.Line\n");
        scanf("%d",&i);
        if(i==1)
        {
            int char_count;
            printf("how many character you want delete : ");
            scanf("%d",&char_count);
            deleteCharacters(editor,char_count);
            return;
        }
        else if(i==2)
        {
            deleteLine(editor);
        }
    }
    else if((strcmp(str,"undo"))==0)
    {
        
        undo(editor); 
        pushStack(&redoStack,undoStack.actions[undoStack.size]);
        undoStack.size--;
    }
    else if((strcmp(str,"redo"))==0)
    {
        if(redoStack.size == -1)
        {
            return;
        }
        redo(editor);
        pushStack(&undoStack,redoStack.actions[redoStack.size--]);
    }
    else if((strcmp(str,"navigation"))==0)
    {
        navigation(editor);
    }
    else 
    {
        printf("Invalid input\n");
    }
}
void insertText(TextEditor *editor, char text[])
{
    Node* n1 = malloc(1*sizeof(Node));
    n1->next = NULL;
    n1->prev=NULL;
    int flag=0;
    if(text==NULL)
    {
        if(editor->cursorLine==0 && editor->cursorPos==0)
        {
            editor->head=n1;
            editor->tail=n1;
            editor->cursor = n1;
            editor->cursorLine++;
            editor->cursorPos=0;
            strcmp(editor->cursor->line,"\0");
            flag=2;
        }
        else
        {        
            Node *temp = editor->cursor->next;
            
            editor->cursor->next = n1;
            
            n1->prev = editor->cursor;
           
            n1->next = temp;
            
            if(n1->next!=NULL)
            n1->next->prev = n1;
            
            editor->cursor = n1;
            editor->cursorLine++;
            editor->cursorPos=0;
            strcmp(editor->cursor->line,"\0");
            flag = 2;
           // }
        }
    }
    else if(editor->head==NULL && editor->tail==NULL)
    {
        editor->head=n1;
        editor->tail=n1;
        editor->cursor = n1;
        strcpy(editor->head->line,text);
        editor->cursorLine++;
        editor->cursorPos += strlen(text);
        flag =1;
    }
    else
    {
        //char line[256];
        int len = strlen(text);
        strcat(text,(editor->cursor->line)+editor->cursorPos);
        editor->cursor->line[editor->cursorPos]='\0';
        strcat(editor->cursor->line,text);
        editor->cursorPos+=len;
        flag =1;

    }
    if(flag != 0)
    {
        if(flag == 2)
        {
            Action temp;
            temp.cur = editor->cursor;
            temp.cursorLine = editor->cursorLine;
           
            //editor->cursorLine--;
            strcpy(temp.operation,"insert");
            strcpy(temp.text,"\n");
            temp.cursorPos = 0;
            //printf("shubham jagtap\n");
            pushStack(&undoStack,temp);
        }
        else
        {
            Action temp;
            temp.cur = editor->cursor;
            temp.cursorLine = editor->cursorLine;
           
            //editor->cursorLine--;
            strcpy(temp.operation,"insert");
            strcpy(temp.text,text);
            temp.cursorPos = editor->cursorPos - strlen(text);
            //printf("shubham jagtap\n");
            pushStack(&undoStack,temp);
        }
    }
    printf("text : %s\n %d\n%d\n",editor->cursor->line,editor->cursorLine,editor->cursorPos);
}
void displayText(TextEditor *editor)
{
    Node *temp = editor->head;
    if(temp == NULL)
    {
        printf("No text to print\n");
        return;
    }
    while(temp!=NULL)
    {
        if(temp->line[0]!='!')
            printf("%s\n",temp->line);
        temp = temp->next;
    }
    printf("CursorLine: %d\tCursorPos: %d\n",editor->cursorLine,editor->cursorPos);
}
