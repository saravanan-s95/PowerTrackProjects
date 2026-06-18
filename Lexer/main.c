#include <stdio.h>
#include "stack.h"
#include "token_spec.h"
#include "classifier.h"
#include "helper.h"

Stack stk;

int main(int argc, char *argv[]){

    validate_file_name(argv[1]);
    init_stack(&stk);


    while(1){

        // Testing Classifier
        Token t = get_next_token();

        if(t.token_type == END_OF_FILE || t.token_type == UNKNOWN){
            printf("< Lexeme: %-25s Type: %-15s >\n", t.lexme, token_name[t.token_type]);
            break;
        }

        printf("< Lexeme: %-25s Type: %-15s >\n", t.lexme, token_name[t.token_type]);
    }

    // Check if any brackets are missing
    if(stk.top >= 0){
        printf("\n====================ERRORS=====================\n\n");
        printf("E: No matching brackets found: ");
        for(int i = 0; i <= stk.top; i++){
            printf("%c ", stk.values[i]);
        }

        printf("\n\n");
    }



    return 0;
}