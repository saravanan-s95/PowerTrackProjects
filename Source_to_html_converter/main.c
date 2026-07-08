#include "header.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        printf("Error: Invalid Arguments\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen("output.html", "w");

    if (!in || !out)
    {
        printf("Error: File error\n");
        return 1;
    }

    // HTML Header
    fprintf(out,
            "<html><head><style>"
            ".preprocessor{color:#0bb09d;}"
            ".header{color:#b00b9f;}"
            ".keyword_data{color:#09f705;}"
            ".keyword_non_data{color:#f7ef05;}"
            ".comment{color:#0bb09d;}"
            ".string{color:#b00b9f;}"
            ".number{color:#b00b9f;}"
            ".char{color:#b00b9f;}"
            "</style></head><body><pre>\n");

    char ch;

    while ((ch = fgetc(in)) != EOF)
    {
        if (ch == '#')
        {
            handle_preprocessor(in, out, ch);
        }
        else if (isalpha(ch) || ch == '_')
        {
            handle_identifiers(in, out, ch);
        }
        else if (isdigit(ch))
        {
            handle_numbers(in, out, ch);
        }
        else if (ch == '"')
        {
            handle_string(in, out, ch);
        }
        else if (ch == '/')
        {
            handle_comment(in, out, ch);
        }
        else if (ch == '\'')
        {
            handle_char(in, out, ch);
        }
        else
        {
            // Preserve HTML special characters
            if (ch == '<')
                fprintf(out, "&lt;");
            else if (ch == '>')
                fprintf(out, "&gt;");
            else
                fputc(ch, out);
        }
    }

    fprintf(out, "</pre></body></html>");

    fclose(in);
    fclose(out);

    printf("Conversion completed. Check output.html\n");

    return 0;
}
