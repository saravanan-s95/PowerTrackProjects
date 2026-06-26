/*
NAME		: Triveni
DATE		: 21-06-2026
PROJECT 	: LEXICAL ANALYZER
Description : Reads a C source file, identifies tokens,
              classifies them and performs basic syntax
              checking for parentheses and braces.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// List of C keywords
char keywords[][10] = {
    "int", "float", "char", "double", "if", "else", "while",
    "for", "return", "void", "break", "continue", "switch",
    "case", "default", "do", "struct", "long", "short", "unsigned"
};

// Function to check whether a token is a keyword
int isKeyword(char str[]) {
    int n = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < n; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;   // Keyword found
    }
    return 0;           // Not a keyword
}

int main() {
    FILE *fp;
    char ch, token[MAX];
    int i = 0;

    // Counters for checking balanced parentheses and braces
    int paren = 0, brace = 0;

    // Open the C source file
    fp = fopen("input.c", "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("Token Classification:\n\n");

    // Read the file character by character
    while ((ch = fgetc(fp)) != EOF) {

        // Count opening and closing parentheses
        if (ch == '(')
            paren++;
        else if (ch == ')')
            paren--;

        // Count opening and closing braces
        else if (ch == '{')
            brace++;
        else if (ch == '}')
            brace--;

        // Check for identifiers or keywords
        if (isalpha(ch) || ch == '_') {

            i = 0;
            token[i++] = ch;

            // Read the complete identifier/keyword
            while ((ch = fgetc(fp)) != EOF &&
                   (isalnum(ch) || ch == '_')) {
                token[i++] = ch;
            }

            token[i] = '\0';

            // Classify as keyword or identifier
            if (isKeyword(token))
                printf("%-15s : Keyword\n", token);
            else
                printf("%-15s : Identifier\n", token);

            // Put back the extra character into the file
            if (ch != EOF)
                ungetc(ch, fp);
        }

        // Check for numeric constants
        else if (isdigit(ch)) {

            i = 0;
            token[i++] = ch;

            // Read the complete number
            while ((ch = fgetc(fp)) != EOF &&
                   isdigit(ch)) {
                token[i++] = ch;
            }

            token[i] = '\0';

            printf("%-15s : Number\n", token);

            // Put back the extra character
            if (ch != EOF)
                ungetc(ch, fp);
        }

        // Check for operators
        else if (strchr("+-*/%=<>!", ch)) {
            printf("%-15c : Operator\n", ch);
        }

        // Check for special symbols
        else if (strchr(";:,[]()", ch)) {
            printf("%-15c : Special Symbol\n", ch);
        }
    }

    // Close the file
    fclose(fp);

    // Display syntax checking result
    printf("\nSyntax Checking:\n");

    if (paren == 0)
        printf("Parentheses are balanced.\n");
    else
        printf("Parentheses are NOT balanced.\n");

    if (brace == 0)
        printf("Braces are balanced.\n");
    else
        printf("Braces are NOT balanced.\n");

    return 0;
}