#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS    35
#define MAX_TOKEN_SIZE  100

#define SUCCESS         1
#define FAILURE         -1

#define TRUE            0
#define FALSE           1


typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    UNKNOWN
} TokenType;


typedef struct stack
{
    unsigned int capacity;
    int top;
    char* arr;
}stack_t;


int validate_filename(char* file);
int read_data_from_file_type_analysis(FILE* fptr);

int separate_token(char* str);
int is_token_delimiter(char ch);

void categorizeToken(char* token);
int is_preprocessor_directive(char* str);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(const char* ch);
int isConstant(const char* str);


void read_data_from_file_find_error(FILE* fptr);

int find_error(char* str, int line_count);
int check_quotes(char* str);
int check_curved_bracket(char* str);

int flower_bracket_stack(char* str);
int check_flower_bracket(void);
void push(char ch);
void pop(void);

int is_hexa_valid(char* str);
int is_octal_valid(char* str);

#endif
