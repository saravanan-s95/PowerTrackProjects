
#include "lexer.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>

static char* res_kwords_data[] = {"const", "volatile", "extern", "auto", "register",
                                  "static", "signed", "unsigned", "short", "long",
                                  "double", "char", "int", "float", "struct",
                                  "union", "enum", "void", "typedef", ""
                                 };

static char* res_kwords_non_data[] = {"goto", "return", "continue", "break",
                                      "if", "else", "for", "while", "do",
                                      "switch", "case", "default", "sizeof", ""
                                     };

static char operators[] = {'/', '+', '*', '-', '%', '=', '<', '>', '~', '&', ',', '!', '^', '|','\0'};
static char braces[] = {'(', ')', '{', '}', '[', ']',  ',',  ';',  '\0'};

void initializeLexer(FILE *fp)
{
    char ch = fgetc(fp);
    if (ch == EOF) {
    printf("The file is empty.\n");
    return;
    }
    rewind(fp);
    char line_buffer[1024];
    while (fgets(line_buffer, sizeof(line_buffer), fp) != NULL)
    {
      char ch;
      char word[50];
      int idx =0;
      if(line_buffer[0]=='#'){
        int i=0;
        for(i=0;line_buffer[i] != '\n';i++);
        line_buffer[i-1]='\0';
        printf("\t %-15s : PREPROCESSOR_DIRECTIVE\n",line_buffer);
        continue;
        }
        int sl =0;
        for(int i=0; line_buffer[i] != '\0'; i++){
        
            char ch = line_buffer[i];
            if(ch == '/' && line_buffer[i+1] == '/'){
                printf("\t //              : COMMENT\n");
                break;
            }
            if(ch == '/' && line_buffer[i+1] == '*'){
                printf("\t /*              : COMMENT\n");
                break;
            }
            if(ch =='\n') continue;
            if(ch == '"'){
                if(sl == 0){
                    sl = 1;
                    word[idx++] = ch;
                }else{
                    sl = 0;
                    word[idx++] = ch;
                    word[idx] = '\0';
                    Token token;
                    strcpy(token.lexeme,word);
                    categorizeToken(&token);
                    printf("\t %-15s : %s\n",token.lexeme,getTokenString(token.type));
                    idx = 0;
                }
                continue;
            }
            if(sl == 1){
                word[idx++] = ch;
                continue;
            }
            if(isSpecialCharacter(ch) || isOperator(&ch)){
                if(idx > 0){
                    word[idx] = '\0';
                    Token token;
                    strcpy(token.lexeme,word);
                    categorizeToken(&token);
                    printf("\t %-15s : %s\n",token.lexeme,getTokenString(token.type));
                    idx = 0;
                }
                Token token;
                token.lexeme[0] = ch;
                token.lexeme[1] = '\0';
                categorizeToken(&token);
                if(token.type == SPECIAL_CHARACTER){
                    printf("\t %-15s : %s\n", token.lexeme,specialchars(ch));
                }else
                printf("\t %-15s : %s\n", token.lexeme,getTokenString(token.type));
            }
            else if(ch == ' ' ||ch == '\n' ||ch== '\t' || ch == ';'){
                if(idx > 0){
                    word[idx] = '\0';
                    Token token;
                    strcpy(token.lexeme,word);
                    categorizeToken(&token);
                    printf("\t %-15s : %s\n", token.lexeme,getTokenString(token.type));
                    idx = 0;
                }
            } else {
                word[idx++] = line_buffer[i];
            }
        }
               
    }
}

void categorizeToken(Token *token)
{
    if(isKeyword(token->lexeme)){
        token->type = KEYWORD;
    }else if(isOperator(token->lexeme)){
        token->type = OPERATOR;
    }else if(isSpecialCharacter(token->lexeme[0])){
        token->type = SPECIAL_CHARACTER;
    }else if(isConstant(token->lexeme)){
        token->type = CONSTANT;
    }else if(isIdentifier(token->lexeme)){
        token->type = IDENTIFIER;
    }else if(isComment(token->lexeme)){
        token->type = COMMENT;
    }else if(isPreprocessorDirective(token->lexeme)){
        token->type = PREPROCESSOR_DIRECTIVE;
    }else if(isStringLiteral(token->lexeme)){
        token->type = STRING_LITERAL;
    }
    else {
        token->type = UNKNOWN;
    }
}
char *getTokenString(int num){
    if(num == 0){
        return "KEYWORD";
    }else if(num ==1) return "OPERATOR";
    else if(num == 2) return "SPECIAL_CHARACTER";
    else if(num == 3) return "CONSTANT";
    else if(num == 4) return "IDENTIFIER";
    else if(num == 5) return "PREPROCESSOR_DIRECTIVE";
    else if(num == 6) return "COMMENT";
    else if(num == 7) return "STRING_LITERAL";
    else return "UNKNOWN";
}

int isKeyword(const char* str)
{
    for(int i = 0;res_kwords_data[i][0] != '\0';i++){
        if(strcmp(str, res_kwords_data[i])==0){
            return 1;
        }
    }
    for(int i = 0;res_kwords_non_data[i][0] != '\0';i++){
        if(strcmp(str, res_kwords_non_data[i]) == 0){
            return 1;
        }
    }
    return 0;
}
char *specialchars(char ch){
    if(ch == '(') return "open parenthesis";
    else if(ch == ')') return "close parenthesis";
    else if(ch == '{') return "open brace";
    else if(ch == '}') return "close brace";
    else if(ch == '[') return "open bracket";
    else if(ch == ']') return "close bracket";
    else if(ch == ',') return "comma";
    else if(ch == ';') return "semicolon";
    else return "unknown special character";
}
int isSpecialCharacter(char ch)
{
    for(int i = 0; braces[i] != '\0'; i++){
        if(ch == braces[i]){
            return 1;
        }
    }
    return 0;
}
int isStringLiteral(const char* str)
{
    int len = strlen(str);
    return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}
int isConstant(const char* str)
{
    int i = 0;
    if(str[0] == '-'||str[0]=='+'){
        i++;
    }
    for(i;str[i]!= '\0'; i++){
        if(!isdigit(str[i])){
            return 0;
        }
    }
    return 1;
}

int isIdentifier(const char* str){
    if(!isalpha(str[0]) && str[0] != '_'){
        return 0;
    }
    for(int i = 1; str[i] != '\0'; i++){
        if(!isalnum(str[i]) && str[i] != '_'){
            return 0;
        }
    }
    return 1;
}
int isOperator(const char* str)
{
    for(int i = 0; operators[i] != '\0'; i++){
        if(str[0] == operators[i]){
            return 1;
        }
    }
    return 0;
}
int isPreprocessorDirective(const char* str)
{
    return str[0] == '#';
}
int isComment(const char* str)
{
    return (str[0] == '/' && str[1] == '/') || (str[0] == '/' && str[1] == '*');
}