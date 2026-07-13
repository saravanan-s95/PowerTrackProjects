#include<stdio.h>
char *keywords[] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

char operators[] = "+-*/=%<>!";

char *relational_operators[] = {
    "+", "-", "*", "/", "=",
    "==", "!=", ">=", "<=",
    "&&", "||", "++", "--"
};
int num_relational_operators = sizeof(relational_operators) / sizeof(relational_operators[0]);
char special[] = ";:,(){}[]";