#ifndef LEXER_H 
#define LEXER_H 

#include <stdio.h> 

extern int line_no; // Variable to track current line number

void tokenize(FILE *fp); // Function to start tokenizing the source file

void readIdentifier(FILE *fp, char first); // Function to read identifiers and keywords

void readNumber(FILE *fp, char first); // Function to read integer and floating numbers

void readOperator(FILE *fp, char first); // Function to identify operators and delimiters

void readString(FILE *fp); // Function to read string literals

void skipComment(FILE *fp); // Function to skip single line comments

int isOperator(char ch); // Function to check whether the character is an operator

int isDelimiter(char ch); // Function to check whether the character is a delimiter

int isKeyword(char *word); // Function to check whether the word is a keyword

void lexicalError(int line, char *message); // Function to display lexical errors

void displayErrors(); // Function to display all lexical errors

void checkBrackets(); // Function to check brackets and parenthesis

int isSpecialLine(char *line); // Function to check lines where semicolon is not required

#endif
