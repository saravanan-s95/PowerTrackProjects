#include "lexer.h"

/* Reserved keywords as listed in the project requirements */
char *type_keywords[] = { "const", "volatile", "extern", "auto", "register", "static",
                          "signed", "unsigned", "short", "long", "double", "char", "int", "float",
                          "struct", "enum", "union", "void", "typedef", NULL};

/* Non-reserved keywords as listed in the project requirements */
char *control_keywords[] = { "if", "else", "switch", "case", "goto", "default",
                             "break", "continue", "for", "do", "while", "return", "sizeof", NULL};

char *directives[] = { "#include", "#define", "#if", "#endif", "#ifdef", "#ifndef",
                       "#undef", "#else", "#elif", "#pragma", NULL};

char operators[] = { '/', '*', '-', '+', '%', '=', '<', '>', '&', '!', '^', '|', '~', ',', '\0'};

char *compound_operators[] = { "==", "!=", "|=", "&=", "&&", "||", "<<=", ">>=", "^=", "<=", ">=",
                               "+=", "-=", "*=", "/=", "%=", "<<", ">>", "++", "--", "->", NULL};

char symbol[] = { '(', ')', '{', '}', '[', ']', ':', ';', '.', '?', '\0'};
