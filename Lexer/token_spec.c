#include "token_spec.h"
#include "buff_manager.h"
#include <string.h>
#include "stack.h"

extern Stack stk;

// Language: [_a-zA-Z]
bool letter(void){

    char temp = peek();
    
    if((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z')){
        advance();
        return 1;
    }

    return 0;
}


// Language: [0-9]
bool number(void){

    char temp = peek();

    if(temp >= '0' && temp <= '9'){
        advance();
        return 1;
    }

    return 0;
}


// Language: [' ', \n, \t]
bool whitespace(void){
    char temp = peek();

    if(temp == ' ' || temp == '\t' || temp == '\n'){
        advance();
        return 1;
    }

    return 0;
}


// Language: (letter)(letter/number)*
bool identifier(void){
    if(letter()){
        while(letter() || number());

        return 1;
    }

    return 0;
}


// Language: {C Keywords}
bool keyword(char *str){

    for(unsigned int i = 0; i < KEYWORDS; i++){
        if(strcmp(str, keywords[i]) == 0){
            return 1;
        }
    }    

    return 0;
}


// Language: {C Operators}
bool opr(void){
    char first = peek();
    if(strchr(operators, first)){
        advance();

        char second = peek();

        // Lookahead for ++, --, shorthand assignment
        if((first == '+' && second == '+' )||  
            (first == '-' && second == '-')||
            second == '=') advance();
        
        // Lookahead for logical operators
        if((first == '|' && second == '|') ||
            (first == '&' && second == '&')) advance();

        return 1;
    }

    return 0;
}


// Language: {C Special chars}
bool special_chars(void){
    char temp = peek();
    
    if(strchr(special_characters, temp)){

        // If the special char is (, { or [ push into stack
        if(temp == '(' || temp == '{' || temp == '['){
            push(&stk, temp);
        }
        else{
            char tos = top(&stk);

            if((temp == ')' && tos == '(') || (temp == '}' && tos == '{') || (temp == ']' && tos == '[')){
                pop(&stk);
            }
        }

        advance();
        return 1;
    }

    return 0;
}


// Language: {Integer and float literal}
int integer_or_float(void){

    if(number()){
        while(number());

        if(peek() == '.'){ 
            advance();
        }
        else{ 
            return 1;
        }
        
        while(number());

        return 2;
    }

    return 0;
}


// Language: {Char and string literal}
int char_or_string(void){

    if(peek() == '\''){
        advance(); // '
        advance(); // char
        advance(); // '

        return 1;
    }
    else if(peek() == '\"'){

        advance(); // "

        while(peek() != '\"'){
            advance();
        }

        advance(); // "

        return 2;
    }

    return 0;
}


bool preprocessor(void){
    if(peek() == '#'){
        advance();

        while(peek() != '\n') advance();

        return 1;
    }

    return 0;
}