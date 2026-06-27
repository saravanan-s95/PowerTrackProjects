#include <string.h>
#include<ctype.h>


#define MAX_KEYWORDS 24


static const char *keywords[] = {
        "int","float","char","if","else","for","while","do","return",
        "void","double","long","short","switch","case","break","continue",
        "struct","typedef","union","static","const","unsigned","signed"
    };

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";


int isKeyword(const char* str);
int isOperator(char ch);
int isSpecialCharacter(char ch);