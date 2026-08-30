#include "types.h"
#include "lexical.h"

Status lexical_analyser(File_List **head, Stack *stk)
{
    File_List *temp = *head;
    stk->top = -1;
    stk->arr = malloc(sizeof(char) * 1000);

    while (temp)
    {
        // Opening the file
        FILE *fp = fopen(temp->f_name, "r");

        if (fp == NULL)
        {
            printf("ERROR: %s file cannot be opened\n", temp->f_name);
            return e_failure;
        }

        // Printing character by character
        int ch;
        char word[10000]; // Having a temp buffer to store a word
        int k = 0;
        while ((ch = fgetc(fp)) != EOF)
        {

            // If character is alphabet or a underscore then add it into buffer and keep building the word
            if (isalpha(ch) || ch == '_')
                word[k++] = ch;

            // If character is a number then classify is as "number"
            else if (isnumber(ch))
                printf("%c -> Number\n", ch);

            // Operator Handling
            else if (ch == '+')
            {
                int next = fgetc(fp);

                if (next == '+')
                    printf("++ -> Operator\n");
                else if (next == '=')
                    printf("+= -> Operator\n");
                else
                {
                    printf("+ -> Operator\n");
                    ungetc(next, fp);
                }
            }

            else if (ch == '=')
            {
                // First processing pending word already in the buffer
                if (k > 0)
                {
                    word[k] = '\0';

                    if (is_keyword(word) == e_success)
                        printf("%s -> Keyword\n", word);

                    else
                        printf("%s -> Identifier\n", word);

                    k = 0;
                }
                int next = fgetc(fp);

                if (next == '=')
                    printf("== -> Operator\n");

                else
                {
                    printf("= -> Operator\n");
                    ungetc(next, fp);
                }
            }

            else if (ch == '>')
            {
                int next = fgetc(fp);

                if (next == '=')
                    printf(">= -> Operator\n");

                else
                {
                    printf("> -> Operator\n");
                    ungetc(next, fp);
                }
            }

            else if (ch == '<')
            {
                int next = fgetc(fp);

                if (next == '=')
                    printf("<= -> Operator\n");

                else
                {
                    printf("< -> Operator\n");
                    ungetc(next, fp);
                }
            }

            else if (is_symbol(ch) == e_success)
            {
                if (k > 0)
                {
                    // First handling word which was made in the buffer before encountering symbol otherwise word made like main(, here main will get thrown away
                    word[k] = '\0';

                    if (is_keyword(word) == e_success)
                        printf("%s -> Keyword\n", word);

                    else
                        printf("%s -> Identifier\n", word);

                    k = 0;
                }
                if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
                {
                    Status ret = parantheses_handler(stk, ch, temp->f_name);
                    if (ret != e_success)
                        return e_failure;
                }
                printf("%c -> Symbol\n", ch);
            }

            // Handling preprocessor directives
            else if (ch == '#')
            {
                k = 0;
                word[k++] = ch;

                while ((ch = fgetc(fp)) != EOF && ch != '\n')
                {
                    word[k++] = ch;
                }

                word[k] = '\0';

                printf("%s -> Preprocessor Directive\n", word);

                continue;
            }

            // To handle comments
            else if (ch == '/')
            {
                int next = fgetc(fp);

                if (next == '/')
                {
                    while ((ch = fgetc(fp)) != '\n')
                        ;

                    continue;
                }

                else if (next == '*')
                {
                    int prev = 0;

                    while ((ch = fgetc(fp)) != EOF)
                    {
                        if (prev == '*' && ch == '/')
                            break;

                        prev = ch;
                    }

                    continue;
                }


                ungetc(next, fp);
            }

            // To handle string literals
            else if (ch == '"')
            {
                k = 0;
                word[k++] = ch;

                while ((ch = fgetc(fp)) != '"')
                {
                    word[k++] = ch;
                }

                word[k++] = '"';
                word[k] = '\0';

                printf("%s -> String Literal\n", word);

                k = 0; // To clear the buffer
                continue;
            }

            // If we encounter any space or new line
            else
            {
                // k > 0 cond is used to stop making spaces as words
                if (k > 0)
                {
                    // Add null char to end the word
                    word[k++] = '\0';

                    // Classification of keyword and identifiers
                    if (isword(word) == e_success)
                    {
                        if (is_keyword(word) == e_success)
                            printf("%s -> Keyword\n", word);

                        else
                            printf("%s -> Identifier\n", word);
                    }
                    k = 0;
                }
            }
        }
        temp = temp->link;
    }

    return e_success;
}
