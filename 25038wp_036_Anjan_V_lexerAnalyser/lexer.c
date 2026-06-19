#include "main.h"

/* returns 1 if keyword found, otherwise 0 */
int isKeyword(const char* str){
    
    for(unsigned int i = 0; i < MAX_KEYWORDS; i++)
    {
        /* check if keyword exists in token */
        if(strstr(str, keywords[i]) != NULL){
            return 1;
        }
    }

    return 0;
}

/* returns 1 if operator found, otherwise 0 */
int isOperator(char ch){

    if(strchr(operators, ch) != NULL){
        return 1;
    }
    return 0;
}

/* returns 1 if special char found, otherwise 0 */
int isSpecialCharacter(char ch)
{
    return strchr(specialCharacters, ch) != NULL;
}