#include<stdio.h>
#include<string.h>
#include"lexical_analyzer.h"

int validate(char*filename){
    char*addr=strrchr(filename,'.');
    if(addr==NULL||strcmp(addr,".c")!=0){
        return FAILURE;
    }
    return SUCCESS;
}
int main(int argc, char*argv[]){
    
    if(argc==2){
        int status=validate(argv[1]);
        if(status==SUCCESS){
            status=lexer(argv[1]);
            if(status==SUCCESS){
                printf("\nParsing completed.\n");
            }
            else{
                printf("\nParsing failed.\n");
            }
        }
        else{
            printf("Incorrect file extension.\n");
            printf("Usage: ./lexical_analyzer <.c file> \n");
            return FAILURE;
        }
    }
    else{
        printf("Usage: ./lexical_analyzer <.c file> \n");
        return FAILURE;
    }

}