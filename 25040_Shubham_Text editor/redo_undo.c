#include "file_ops.h"
#include "redo_undo.h"
#include "text_ops.h"
#include "curser_navigations.h"
#include "display_search.h"
extern DynamicArrayStack undoStack;
extern DynamicArrayStack redoStack;
void pushStack(DynamicArrayStack *stack,Action data)
{
    if(stack->size >= stack->capacity)
    {
        printf("error stack full\n");
        return;
    }   
    stack->size++;
    strcpy(stack->actions[(stack->size)].operation,data.operation);
    //strcpy((stack->actions[(stack->size)].),data.operation);
    stack->actions[(stack->size)].cur = data.cur;
    stack->actions[(stack->size)].cursorLine = data.cursorLine;
    stack->actions[(stack->size)].cursorPos = data.cursorPos;
    strcpy((stack->actions[(stack->size)].text),data.text);
    // printf("text add stack\n");
   // printf("\n%d\n",stack->size);
}
Action popStack(DynamicArrayStack *stack)
{
    if(stack->size>0)
    {   
        return stack->actions[stack->size--];
    }
}
void undo(TextEditor *editor)
{

    if((strcmp(undoStack.actions[undoStack.size].operation,"delete"))==0)
    {
        int line = undoStack.actions[undoStack.size].cursorLine;
        //printf("shubham %d\n",undoStack.actions[undoStack.size].cursorLine);
        if(editor->head == NULL)
        {
            insertText(editor,undoStack.actions[undoStack.size].text);
            return;
        }
        // else if((undoStack.actions[undoStack.size].cursorPos)==0)
        // {
        //     Node* n1 = malloc(1*sizeof(Node));
        //     n1->next = NULL;
        //     n1->prev=NULL;
        //     strcpy(n1->line,undoStack.actions[undoStack.size].text);
        //     //strcpy(n1->line,undoStack.actions[undoStack.size].text);

        //     printf("line : %d\n",undoStack.actions[undoStack.size].cursorLine);
        //     Node *temp = editor->cursor->next;
        //     editor->cursor->next = n1;
        //     n1->prev = editor->cursor;
        //     editor->cursor = n1;
        //     editor->cursorLine = undoStack.actions[undoStack.size].cursorPos;
        //     editor->cursorPos +=strlen(undoStack.actions[undoStack.size].text);
        //    // undoStack.size--;
        // }
        // else
        // {
        
        
            Node *n1= undoStack.actions[undoStack.size].cur;
            if(n1->line[0]=='!')
            {
                n1->line[0]='\0';
            }
            int pos = undoStack.actions[undoStack.size].cursorPos;
            editor->cursor = n1;
            strcat(n1->line+pos,undoStack.actions[undoStack.size].text);
            editor->cursorLine = undoStack.actions[undoStack.size].cursorLine;
            editor->cursorPos = strlen(editor->cursor->line);
            //undoStack.actions[undoStack.size--];
       // }
        //strcpy(undoStack.actions[undoStack.size].operation,"insert");
        return; 
    }
    if((strcmp(undoStack.actions[undoStack.size].operation,"insert"))==0)
    {
        if(undoStack.actions[undoStack.size].text[0] == '\n')
        {
            if(editor->cursor->next == NULL)
            {
                free(editor->cursor);
                editor->cursor = NULL;
                editor->cursorLine=0;
                editor->cursorPos=0;
                editor->head =NULL;
                editor->tail = NULL;
                // undoStack.size--;

                return;
            }
           // Node *temp = editor->cursor;
            //editor->cursor->prev->next = editor->cursor->next;
           // printf("jagtap\n");
            // if(editor->cursor->next!=NULL)
            //     editor->cursor->next->prev = editor->cursor->prev;
            strcpy(editor->cursor->line,"!");
            if(editor->cursor->prev!=NULL)
               editor->cursor = editor->cursor->prev;
            else 
               editor->cursor = editor->cursor->next;

            
            editor->cursorLine--;
            editor->cursorPos=0;
            // undoStack.size--;
            //free(temp);    
        }
        else
        {
            int len = (strlen(undoStack.actions[undoStack.size].text));
            int i=0;
            
            for(i=0;i<len;i++)
            {
                
                int j=undoStack.actions[undoStack.size].cursorPos;
                while(undoStack.actions[undoStack.size].cur->line[j])
                {
                    undoStack.actions[undoStack.size].cur->line[j]=undoStack.actions[undoStack.size].cur->line[j+1];
                    j++;
                }
                ///i++;
            }
            editor->cursor = undoStack.actions[undoStack.size].cur;
            editor->cursorPos = undoStack.actions[undoStack.size].cursorPos;
            // undoStack.size--;
        }
        
    }
}
void redo(TextEditor *editor)
{
    if(redoStack.size == -1)
    {
        return;
    }
    if(strcmp(redoStack.actions[redoStack.size].operation,"insert") == 0)
    {
        int line = redoStack.actions[redoStack.size].cursorLine;
        //printf("shubham %d\n",undoStack.actions[undoStack.size].cursorLine);
        if(editor->head == NULL)
        {
            insertText(editor,redoStack.actions[redoStack.size].text);
        }
        else if((redoStack.actions[redoStack.size].cursorPos)==0)
        {
            //printf("katare\n");
            if(redoStack.actions[redoStack.size].cur != NULL)
            {
                strcpy(redoStack.actions[redoStack.size].cur->line,redoStack.actions[redoStack.size].text);
                return;
            }
        //     Node* n1 = malloc(1*sizeof(Node));
        //     n1->next = NULL;
        //     n1->prev=NULL;
        //     strcpy(n1->line,redoStack.actions[redoStack.size].text);
        //     //strcpy(n1->line,undoStack.actions[undoStack.size].text);

        //    // printf("line : %d\n",redoStack.actions[redoStack.size].cursorLine);
        //     Node *temp = editor->cursor->next;
        //     editor->cursor->next = n1;
        //     n1->prev = editor->cursor;
        //     editor->cursor = n1;
        //     editor->cursorLine = redoStack.actions[redoStack.size].cursorPos;
        //     editor->cursorPos +=strlen(redoStack.actions[redoStack.size].text);
        //    // redoStack.size--;
        // }
        // else
        // {
            Node *n1= redoStack.actions[redoStack.size].cur;
            int pos = redoStack.actions[redoStack.size].cursorPos;
                
            strcat(n1->line+pos,redoStack.actions[redoStack.size].text);
            editor->cursorLine = redoStack.actions[redoStack.size].cursorLine;
            editor->cursorPos = strlen(editor->cursor->line);
            //redoStack.actions[redoStack.size--];
        //}
        
        return; 
    }
}
    else if(strcmp(redoStack.actions[redoStack.size].operation,"delete") == 0)
    {
        if(redoStack.actions[redoStack.size].text[0] == '\n')
        {
            if(editor->cursor->next == NULL)
            {
                free(editor->cursor);
                editor->cursor = NULL;
                editor->cursorLine=0;
                editor->cursorPos=0;
                editor->head =NULL;
                editor->tail = NULL;
                // undoStack.size--;

                return;
            }
            Node *temp = editor->cursor;
            //editor->cursor->prev->next = editor->cursor->next;
            //printf("jagtap\n");
            // if(editor->cursor->next!=NULL)
            //     editor->cursor->next->prev = editor->cursor->prev;
            strcpy(editor->cursor->line,"!");
            if(editor->cursor->prev!=NULL)
               editor->cursor = editor->cursor->prev;
            else 
               editor->cursor = editor->cursor->next;
            //editor->cursor = editor->cursor->prev;
            editor->cursorLine--;
            editor->cursorPos=0;
            
            // undoStack.size--;
           // free(temp);    
        }
        else
        {
            int len = (strlen(redoStack.actions[redoStack.size].text));
            int i=0;    
            for(i=0;i<len;i++)
            {    
                int j=redoStack.actions[redoStack.size].cursorPos;
                while(redoStack.actions[redoStack.size].cur->line[j])
                {
                    redoStack.actions[redoStack.size].cur->line[j]=redoStack.actions[redoStack.size].cur->line[j+1];
                    j++;
                }
            }
            editor->cursorPos = redoStack.actions[redoStack.size].cursorPos;
        }
        
    
    }
}