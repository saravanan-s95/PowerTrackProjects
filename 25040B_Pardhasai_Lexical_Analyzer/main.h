#ifndef MAIN_H
#define MAIN_H

/* ANSI Color Codes */
#define RESET       "\033[0m"

#define BLUE        "\033[34m"   // Line Number
#define YELLOW      "\033[33m"   // Lexeme

#define MAGENTA     "\033[35m"   // Preprocessor
#define BOLD        "\033[1m"
#define CYAN        "\033[36m"   // Header File
#define GREEN       "\033[32m"   // Keyword
#define WHITE       "\033[37m"   // Identifier
#define BRIGHT_BLUE "\033[1;34m" // Number
#define RED         "\033[31m"   // Operator
#define BRIGHT_CYAN "\033[1;36m" // Special Symbol
#define BRIGHT_GREEN "\033[1;32m"// String Literal
#define BRIGHT_RED  "\033[1;31m" // Invalid Token



#define VALID_LEXEME               0

/* Numeric constant errors */
#define MULTIPLE_DOTS             1
#define INVALID_NUMERIC_CONSTANT  2

/* Identifier errors */
#define INVALID_IDENTIFIER        3

/* String and character literal errors */
#define UNTERMINATED_STRING       4
#define INVALID_CHARACTER_CONST   5

/* Illegal characters and operators */
#define ILLEGAL_CHARACTER         6
#define INVALID_OPERATOR          7

/* Bracket errors */
#define MISSING_RPAREN            8
#define EXTRA_RPAREN              9

#define MISSING_RBRACE            10
#define EXTRA_RBRACE              11

#define MISSING_RBRACKET          12
#define EXTRA_RBRACKET            13


#define KEYWORD        1
#define IDENTIFIER     2
#define NUMBER         3
#define OPERATOR       4
#define SPECIAL_SYMBOL 5
#define INVALID_TOKEN  6
#define STRING_LITERAL 7
#define PREPROCESSOR_DIRECTIVE   8
#define HEADER_FILE              9

#define NUM_KEYWORDS 32
extern char *keywords[];
extern char operators[];
extern char *relational_operators[];
extern int num_relational_operators;
extern char special[];
extern int line_no;


void get_next_lexme(FILE *fp,char lexme[]);
void skip_white_spaces(FILE *ws);
int remove_comments(FILE *fp);
void check_brackets(FILE *fp);
int check_error(char arr[]);
void print_error(int ret,char arr[]);
void print_type(int type,char lexme[]);
int classify_token(char *lexeme);
#endif