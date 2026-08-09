#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"lexical_analyzer.h"

int isKeyword(char*token){
    char *keywords[] = { "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    int n=sizeof(keywords)/sizeof(keywords[0]);
    for(int i=0;i<n;i++){
        
        if(strcmp(token,keywords[i])==0){
            return SUCCESS;
        }
    }
    return FAILURE;
}

int operator_helper(char token){
    char *operators = "+-*/%=!<>&|^~?:.->";
    for(int i=0;i<strlen(operators);i++){
        if(token==operators[i]){
            return SUCCESS;
        }
    }
    return FAILURE;

}
int isOperator(char *token){
    char*arithmetic_op[5]={"+","-","*","/","%"};
    char*relational_op[6]={"<",">","<=",">=","!=","=="};
    char*assignment_op[11]={"=","+=","-=","*=","/=","%=","&=","|=","^=","<<=",">>="};
    char*logical_op[3]={"&&","||","!"};
    char*bitwise_op[6]={"&","|","~","<<",">>","^"};
    char*inc_dec_op[2]={"++","--"};
    char*mem_access_op[2]={".","->"};
    char*ternary_op[2]={"?",":"};

    for(int i=0;i<5;i++){
        if(strcmp(token,arithmetic_op[i])==0){
            printf("%-30s: %s\n","ARITHMETIC_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<11;i++){
        if(strcmp(token,assignment_op[i])==0){
            printf("%-30s: %s\n","ASSIGNMENT_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<6;i++){
        if(strcmp(token,relational_op[i])==0){
            printf("%-30s: %s\n","RELATIONAL_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<3;i++){
        if(strcmp(token,logical_op[i])==0){
            printf("%-30s: %s\n","LOGICAL_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<6;i++){
        if(strcmp(token,bitwise_op[i])==0){
            printf("%-30s: %s\n","BITWISE_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<2;i++){
        if(strcmp(token,inc_dec_op[i])==0){
            printf("%-30s: %s\n","INCREMENT_DECREMENT_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<2;i++){
        if(strcmp(token,mem_access_op[i])==0){
            printf("%-30s: %s\n","MEMBER_ACCESS_OPERATOR",token);
            return SUCCESS;
        }
    }
    for(int i=0;i<2;i++){
        if(strcmp(token,ternary_op[i])==0){
            printf("%-30s: %s\n","TERNARY_OPERATOR",token);
            return SUCCESS;
        }
    }
    return FAILURE;
}

int isBracket(char token){
    if(token=='('){
        printf("%-30s: %c\n","OPEN_PARENTHESIS",token);
    }
    else if(token==')'){
        printf("%-30s: %c\n","CLOSE_PARENTHESIS",token);
    }
    else if(token=='{'){
        printf("%-30s: %c\n","OPEN_CURLY_BRACE",token);
    }
    else if(token=='}'){
        printf("%-30s: %c\n","CLOSE_CURLY_BRACE",token);
    }
    else if(token=='['){
        printf("%-30s: %c\n","OPEN_SQUARE_BRACKET",token);
    }
    else if(token==']'){
        printf("%-30s: %c\n","CLOSE_SQUARE_BRACKET",token);
    }
    else return FAILURE;
    return SUCCESS;
}
int isIntFloatConstant(char*token){
    int is_decimal=0;
    for(int i=0;i<strlen(token);i++){
        if(token[i]=='.'){
            is_decimal=1;
            break;
        }
    }
    if(is_decimal==1){
        printf("%-30s: %s\n","FLOAT_CONSTANT",token);
        return SUCCESS;
    } 
    printf("%-30s: %s\n","INTEGER_CONSTANT",token);
    return SUCCESS;
}
int isCharStrConstant(char*token){
    if(token[0]=='\'' && strlen(token)==3){
        printf("%-30s: %s\n","CHAR_CONSTANT",token);
        return SUCCESS;
    }
    else if(token[0]=='"'){
        printf("%-30s: %s\n","STRING_CONSTANT",token);
        return SUCCESS;
    }
    return FAILURE;
}
int lexer(char*filename){
    printf("Opening %s....\n",filename);
    FILE*fp=fopen(filename,"r");
    if(fp==NULL){
        printf("Error opening the file.\n");
        return FAILURE;
    }
    printf("Parsing %s....\n\n",filename);
    char buf[MAX_TOKEN_SIZE];
    int ch;
    int idx=0;
    while((ch=fgetc(fp))!=EOF){
        //check for multi chars 
        if(isalpha(ch)||ch=='_'){
            while(ch!=EOF && (isalnum(ch)||ch=='_')){
                buf[idx++]=ch;
                ch=fgetc(fp);
            }
            if(ch!=EOF){
                ungetc(ch,fp);
            }
            
            buf[idx]='\0';
            if(isKeyword(buf)==SUCCESS){
                printf("%-30s: %s\n","KEYWORD",buf);
                
            }
            else printf("%-30s: %s\n","IDENTIFIER",buf);   
            idx=0; 
        }
        else if(isdigit(ch)){
            while(ch!=EOF && (isdigit(ch)||ch=='.')){
                buf[idx++]=ch;
                ch=fgetc(fp);
            }
            if(ch!=EOF){
                ungetc(ch,fp);
            }
            buf[idx]=0;
            isIntFloatConstant(buf);
            idx=0;
        }
        else if(ch=='\''||ch=='"'){
            buf[idx++]=ch;
            while((ch=fgetc(fp))!=EOF ){
                buf[idx++]=ch;
                if(ch=='\'' || ch=='"'){
                    break;
                }
            }
            buf[idx]='\0';
            isCharStrConstant(buf);
            idx=0;
        }
        else if(ch=='/'){
            int next_ch;

            if((next_ch=fgetc(fp))!=EOF) {
                if(next_ch=='/'){
                    buf[idx++]=ch;
                    buf[idx++]=next_ch;
                    fscanf(fp,"%[^\n]\n",buf+idx);
                    //printf("%-30s: %s\n","SINGLE_LINE_COMMENT",buf);
                    idx=0;
                    //skip single line comments
                    continue;
                }
                else if(next_ch=='*'){
                    buf[idx++]=ch;
                    buf[idx++]=next_ch;
                    while((ch=fgetc(fp))!=EOF){
                        buf[idx++]=ch;
                        if(ch=='/'){
                            break;
                        }
                    }
                    //printf("%-30s: %s\n","MULTI_LINE_COMMENT",buf);
                    idx=0;
                    //skip multiline comments
                    continue;
                }
                else{
                    ungetc(next_ch,fp);
                }
            } 
        }
        else if(operator_helper((char)ch)==SUCCESS){
            buf[idx++]=ch;
            ch=fgetc(fp);
            
            while(ch!=EOF && (operator_helper(ch)==SUCCESS)){
                buf[idx++]=ch;
                ch=fgetc(fp);
            }
            if(ch!=EOF){
                ungetc(ch,fp);
            }
            buf[idx]='\0';
            isOperator(buf);
            idx=0;
            
        }

        else if(ch=='#'){
            buf[idx++]=ch;
            fscanf(fp,"%[^\n]\n",buf+idx);
            printf("%-30s: %s\n","PREPROCESSOR_DIRECTIVE",buf);
            idx=0;
        }
        else if(isBracket(ch)==SUCCESS){}
        else if(ch==';'){
            printf("%-30s: %c\n","SEMICOLON",ch);
        }
        else if(ch==','){
            printf("%-30s: %c\n","COMMA",ch);
        }
        else if(isspace(ch)){
            continue;
        }
        
    }
    fclose(fp);
    return SUCCESS;
}