#include "token_spec.h"
#include "buff_manager.h"
#include "classifier.h"


extern unsigned int lexeme_start;
extern unsigned int lexeme_end;
extern char buffer[];


Token classify_lexeme(void){

    Token curr_token;
    int temp;

    // Check if NULL or EOF reached
    if(buffer[lexeme_end] == '\0' || buffer[lexeme_end] == EOF){
        curr_token.token_type = END_OF_FILE;
        curr_token.lexme[0] = '\0';

        return curr_token;
    }
 
    // Edge case: When we find a whitespace, we do need to move the
    // lexme_start as well
    while(whitespace()) lexeme_start++;

    if(identifier()){
        curr_token.token_type = IDENTIFIER;
    }

    else if(opr()){
        curr_token.token_type = OPERATOR;
    }

    else if(special_chars()){
        curr_token.token_type = SPECIAL_CHAR;
    }

    else if(temp = integer_or_float()){
        // Change integer_or_float() function's return type to int,
        // 0 - no match, 1 - Int, 2 - float
        if(temp == 1) curr_token.token_type = L_INTEGER;
        else curr_token.token_type = L_FLOAT;
    }

    else if(temp = char_or_string()){
        if(temp == 1) curr_token.token_type = L_CHAR;
        else curr_token.token_type = L_STRING;
    }
    else if(preprocessor()){
        curr_token.token_type = PREPROCESSOR;
    }
    else{
        curr_token.token_type = UNKNOWN;
    }


    // The current lexeme value is from lemexe_state - lexeme_end pos in buff
    unsigned int i = lexeme_start;
    for( ;i < lexeme_end; i++){
        curr_token.lexme[i - lexeme_start] = buffer[i];
    }
    curr_token.lexme[i - lexeme_start] = '\0';

    
    // Resetting for next lexeme
    lexeme_start = lexeme_end;

    // Once you have the lexeme value and the token type is Identifier, check if it is a
    // keyword and change the type
    if(keyword(curr_token.lexme)){
        curr_token.token_type = KEYWORD;
    }
    

    return curr_token;
}