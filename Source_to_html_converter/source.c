#include "header.h"

int isKeyword_data(char *word)
{
    for (int i = 0; keywords_data[i] != NULL; i++)
    {
        if (strcmp(word, keywords_data[i]) == 0)
            return 1;
    }
    return 0;
}

int isKeyword_non_data(char *word)
{
    for (int i = 0; keywords_non_data[i] != NULL; i++)
    {
        if (strcmp(word, keywords_non_data[i]) == 0)
            return 1;
    }
    return 0;
}

void handle_identifiers(FILE *in, FILE *out, char ch)
{
    int i = 0;
    char word[100];
    word[i++] = ch;

    while (isalnum(ch = fgetc(in)) || ch == '_')
        word[i++] = ch;

    word[i] = '\0';
    ungetc(ch, in);

    if (isKeyword_data(word))
        fprintf(out, "<span class=\"keyword_data\">%s</span>", word);
    else if (isKeyword_non_data(word))
        fprintf(out, "<span class=\"keyword_non_data\">%s</span>", word);
    else
        fprintf(out, "%s", word);
}

void handle_numbers(FILE *in, FILE *out, char ch)
{
    char number[100];
    int i = 0;
    number[i++] = ch;

    while (isdigit(ch = fgetc(in)))
    {
        number[i++] = ch;
    }
    number[i] = '\0';
    ungetc(ch, in);
    fprintf(out, "<span class=\"number\">%s</span>", number);
}

void handle_string(FILE *in, FILE *out, char ch)
{
    fprintf(out, "<span class=\"string\">\"");

    char prev = 0;
    while ((ch = fgetc(in)) != EOF)
    {
        fputc(ch, out);
        // To ensure that it's not a escape sequence and its actaully " end of string
        if (prev != '\\' && ch == '"')
            break;
        prev = ch;
    }
    fprintf(out, "</span>");
}

void handle_comment(FILE *in, FILE *out, char ch)
{
    char next = fgetc(in);

    if (next == '/')
    {
        fprintf(out, "<span class=\"comment\">//");
        while ((ch = fgetc(in)) != '\n' && ch != EOF)
            fputc(ch, out);
        fprintf(out, "</span>\n");
    }

    // Multi-line comment
    else if (next == '*')
    {
        fprintf(out, "<span class=\"comment\">/*");

        char prev = 0;

        while ((ch = fgetc(in)) != EOF)
        {
            fputc(ch, out);

            if (prev == '*' && ch == '/')
            {
                break;
            }

            prev = ch;
        }

        fprintf(out, "</span>");
    }
    else
    {
        fputc(ch, out);
        ungetc(next, in);
    }
}

void handle_char(FILE *in, FILE *out, char ch)
{
    fprintf(out, "<span class=\"char\">'");

    char prev = 0;
    while ((ch = fgetc(in)) != EOF)
    {
        fputc(ch, out);
        if (prev != '\\' && ch == '\'')
            break;
        prev = ch;
    }
    fprintf(out, "</span>");
}

void handle_preprocessor(FILE *in, FILE *out, char ch)
{
    int i = 0;
    char word[100];

    fprintf(out, "<span class=\"preprocessor\">#");

    while ((ch = fgetc(in)) != EOF && ch != ' ' && ch != '<' && ch != '"')
    {
        word[i++] = ch;
    }
    word[i] = '\0';
    fprintf(out, "%s</span>", word);

    fprintf(out, "<span class=\"header\">");

    while (ch != '\n' && ch != EOF)
    {
        if (ch == '<')
            fprintf(out, "&lt;");
        else if (ch == '>')
            fprintf(out, "&gt;");
        else
            fputc(ch, out);

        ch = fgetc(in);
    }
    fprintf(out, "</span>\n");
}
