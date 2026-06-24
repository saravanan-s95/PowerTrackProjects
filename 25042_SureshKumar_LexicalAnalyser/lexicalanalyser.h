#ifndef HEADER_H
#define HEADER_H
void lexicalanalyser();

void err_identifier(int line, int col, char *word);

int err_integer(int line, int col, char *word);

int err_float(int line, int col, char *word);

void err_parenthesis(int line, int col, char buffer[], int i);

void err_brace(int line, int col, char buffer[], int i);

void err_bracket(int line, int col, char buffer[], int i);

#endif