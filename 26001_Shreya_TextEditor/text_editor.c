#include<stdio.h>
#include"text_editor.h"
#include"text_ops.h"
#include"display.h"
#include"undo_redo.h"
#include"navigation.h"
#include<string.h>
#include<ctype.h>
/*******************************************************************************************************************************************************************
*Title			: Main
*Description	: This function initializes the text editor, the undo and redo stacks 
                  and optionally loads a document from a file passed as a command-line argument. 
                  It then displays the menu and repeatedly reads commands from the user, 
                  and passes them to the command handler until input ends.
*Prototype		: int main(int argc, char*argv[]);
*Input Parameters	: argc: Number of command-line arguments.
			: argv: Array of command-line argument strings; argv[1], if present, is the file name to load from.
*Output			: Status (0 on normal termination or memory allocation failure, 1 if too many command-line arguments were given)
*******************************************************************************************************************************************************************/
int main(int argc,char*argv[]){

    if(argc>2){
        printf("Invalid arguments\n");
        return 1;
    }

    TextEditor text_editor;
    DynamicArrayStack undo,redo;
    text_editor.head=NULL;
    text_editor.tail=NULL;
    text_editor.cursor=NULL;
    text_editor.cursorLine=0;
    text_editor.cursorPos=0;
    //initialising undo and redo stacks
    undo.size=0;
    undo.capacity=STACK_CAPACITY;
    undo.actions=malloc(STACK_CAPACITY*sizeof(Action));
    if(undo.actions==NULL){
        return 0;
    }
    undo.actions->cursorLine=0;
    undo.actions->cursorPos=0;
    
    redo.size=0;
    redo.capacity=STACK_CAPACITY;
    redo.actions=malloc(STACK_CAPACITY*sizeof(Action));
    if(redo.actions==NULL){
        return 0;
    }
    redo.actions->cursorLine=0;
    redo.actions->cursorPos=0;


     //validate file name
    char*filename=NULL;
    if(argc==2){
        filename=malloc(strlen(argv[1])+1);
        strcpy(filename,argv[1]);
        char*addr=strrchr(filename,'.');
        if(addr==NULL||strcmp(addr,".txt")!=0){
            printf("Loading from file failed\n");
        }
        else{
            if(load_from_file(&text_editor,filename)==1)
            printf("Data loaded from %s successfully.\n\n",filename);
        }
    }
    printMenu();
    char buffer[CMD_LEN+MAX_LEN];

    while(1){
        printf("Enter command: ");
        if(fgets(buffer,sizeof(buffer),stdin)==NULL)
            break;
        buffer[strcspn(buffer,"\n")]='\0';
        printf("\n");
        //separate operation and text
        
        char operation[CMD_LEN]={0};
        char text[MAX_LEN]={0};
        int i=0;
        int space_found=0;
        while(buffer[i]){
            if(buffer[i]==' '){
                space_found=1;
                operation[i++]='\0';
                break;
            }
            operation[i]=buffer[i];
            i++;
        }
        if(space_found==0)
        operation[i]='\0';
        int j=0;

        while(buffer[i]){
            text[j]=buffer[i];
            i++;
            j++;
        }
        text[j]='\0';
        handleCommand(&text_editor,operation,text,&undo,&redo,filename);
    }
    

}
/*******************************************************************************************************************************************************************
*Title			: Print Menu
*Description	: This function displays the list of available text editor commands to the user.
*Prototype		: void printMenu();
*Input Parameters	: None
*Output			: None
*******************************************************************************************************************************************************************/
void printMenu(){
    
    printf("------------------------------TEXT EDITOR MENU------------------------------\n");
    printf("Text Editor Commands:\n"
        "1. insert\n"
        "\t1.1 insert <text> - Insert text at the current cursor position\n"
        "\t1.2 insert - Insert new line\n"
        "2. delete\n"
        "\t2.1 delete <num_chars> - Delete a number of characters from cursor\n"
        "\t2.2 delete - Delete entire line\n"
        "3. navigation - Goto navigation Page\n"
        "4. undo - Undo the last operation\n"
        "5. redo - Redo the last undone operation\n"
        "6. print - Print the current state of the text editor\n"
        "7. save - Save file\n"
        "8. exit - Exit the text editor\n\n");
        printf("----------------------------------------------------------------------------\n");
   
}
/*******************************************************************************************************************************************************************
*Title			: Handle Command
*Description	: This function identifies the operation entered by the user 
                  and calls the corresponding function to carry it out, 
                  passing along the accompanying argument text, the undo and redo stacks and the file
                  name as needed. 
                  If the operation is not recognized, it displays an error along with the list of valid commands.
*Prototype		: void handleCommand(TextEditor*text_editor, char*operation, char*text, DynamicArrayStack*undo, DynamicArrayStack*redo, char*filename);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the command is to be performed.
			: operation: Pointer to the string containing the command name entered by the user.
			: text: Pointer to the string containing any argument text following the command name.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
			: filename: Pointer to the string containing the file name passed at program startup, or NULL if none was given.
*Output			: None
*******************************************************************************************************************************************************************/
void handleCommand(TextEditor*text_editor,char*operation,char*text,DynamicArrayStack*undo,DynamicArrayStack*redo,char*filename){
    if(strcmp(operation,"insert")==0){
        insert_text(text_editor,text,undo,redo);
    }
    else if(strcmp(operation,"delete")==0){
        if(text[0]=='\0'){
            delete_line(text_editor,undo,redo);
        }
        else{
            for(int i=0;text[i];i++){
                if(!isdigit(text[i])){
                    printf("Invalid command\n");
                    return;
                }
            }
            delete_chars(text_editor,text,undo,redo);
        }
        
    }
    else if(strcmp(operation,"navigation")==0){
        navigation(text_editor);
    }
    else if(strcmp(operation,"undo")==0){
        Undo(text_editor,undo,redo);
    }
    else if(strcmp(operation,"redo")==0){
        Redo(text_editor,undo,redo);
    }
    else if(strcmp(operation,"print")==0){
        display(*text_editor);
    }
    else if(strcmp(operation,"exit")==0){
        if(text_editor->head){
            save_and_exit(*text_editor,filename);
        }
        else exit(0);
        
    }
    else if(strcmp(operation,"save")==0){
        if(text_editor->head){
            save_to_file(*text_editor,filename);
        }
        else{
            printf("Empty document\n");
        }
       
    }
    else {
        printf("Incorrect command.\n");
        printf("Text Editor Commands:\n"
        "1. insert <text> - Insert text at the current cursor position\n"
        "2. delete <num_chars> - Delete a number of characters from cursor\n"
        "3. navigation - Goto navigation Page\n"
        "4. undo - Undo the last operation\n"
        "5. redo - Redo the last undone operation\n"
        "6. print - Print the current state of the text editor\n"
        "7. save - Save file\n"
        "8. exit - Exit the text editor\n\n");
    }
}
