#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define MAX_LEXEME_SIZE     100

enum type{
    IDENTIFIER,
    KEYWORD,
    OPERATOR,
    SPECIAL_CHAR,
    L_INTEGER,
    L_FLOAT,
    L_CHAR,
    L_STRING,
    PREPROCESSOR,
    UNKNOWN,
    END_OF_FILE
};

static char *token_name[] = {
    "IDENTIFIER",
    "KEYWORD",
    "OPERATOR",
    "SPECIAL_CHAR",
    "L_INTEGER",
    "L_FLOAT",
    "L_CHAR",
    "L_STRING",
    "PREPROCESSOR",
    "UNKNOWN",
    "END_OF_FILE"
};

// Token: <lexme, type>
typedef struct{
    char lexme[MAX_LEXEME_SIZE];
    unsigned int token_type;
}Token;


Token classify_lexeme(void);

#endif