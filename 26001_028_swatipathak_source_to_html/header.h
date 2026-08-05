#ifndef header
#define header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

static char* keywords_data[] = {"const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned", "short", "long", 
	"double", "char", "int", "float", "struct", "union", "enum", "void", "typedef"};

static char* keywords_non_data[] = {"goto", "return", "continue", "break", "if", "else", "for", "while", "do", "switch", "case", "default", "sizeof"};

static char operators[14] = {'/', '+', '*', '-', '%', '=', '<', '>', '~', '&', ',', '!', '^', '|'};
static char symbols[9] = {';', ',', '(', ')', '{', '}', '[', ']', ':'};

// static int main_round_open=0;
// static int main_curly_open=0;
// static int main_round_close=0;
// static int main_curly_close=0;
// static int inbuilt=0;
// static int open_curly=0;
// static int close_curly=0;
static int curly;
static int open_round=0;
static int close_round=0;
int iskeyword(char word[]);
int is_nondatakeyword(char word[]);
int isoperator(char ch);
int issymbol(char ch);
void html_begin(FILE*);
void html_end(FILE*);

void handle_preprocessor(FILE* fp1,FILE* fp2,char ch);
void handle_strings(FILE* fp1,FILE* fp2,char ch);
void handle_identifiers(FILE* fp1,FILE* fp2,char ch);
void handle_comments(FILE* fp1,FILE* fp2,char ch);
void handle_digits(FILE* fp1,FILE* fp2,char ch);
int validatefile(char**argv);
void handle_char_string(FILE*,FILE*,char);
#endif