#include "header.h"

int iskeyword(char word[]){
    for(int i=0;i<19;i++){
        // printf("keyword");
        if(strcmp(keywords_data[i],word)==0){
            return 1;
        }
    }
    // printf("\nI am returning");
    return 0;
}
int is_nondatakeyword(char word[]){
    // printf("keyword");
    for(int i=0;i<13;i++){
        if(strcmp(keywords_non_data[i],word)==0){
            return 1;
        }
    }
    return 0;
}
int isoperator(char ch){
    for(int i=0;i<14;i++){
        if(operators[i]==ch){
            return 1;
        }
    }
    return 0;
}
int issymbol(char ch){
    for(int i=0;i<9;i++){
        if(symbols[i]==ch){
            return 1;
        }
    }
    return 0;

}

